#include "node.hpp"

Node::Node(node_label_t label)
{
    /*!
     * @brief Base constructor of a Node
     * 
     * @param label A purely cosmetic label for the Node
     * 
     */
    
    this->label = label;
    this->p = NULL;
}

Node::Node(node_label_t label, Point* p)
{
    /*!
     * @brief Constructor of a Node wrt a specified point 
     * 
     * @param label A purely cosmetic label for the Node
     * @param p The Point \p p to be used
     * 
     */

    this->label = label;
    this->p = p;
}

Point* Node::get_point()
{
    /*!
     * @brief Returns the point contained in the Node
     * 
     * @return The \p point of the Node
     * 
     */

    return this->p;
}

void Node::set_point(Point* p)
{
    /*!
     * @brief Overwrites the point contained in the Node
     * 
     * @param p The \p point to change
     * 
     */

    this->p = p;
}

double Node::dist(Node* other)
{    
    /*!
     * @brief Returns the Euclidian distance between two Nodes
     * 
     * @param other The \p other Node to evaluate distance with
     * 
     * @return The Euclidian distance between this Node and the \p other Node
     * 
     */

    Point* p1_p = this->p;
    Point* p2_p = other->get_point();

    if (p1_p != NULL && p2_p != NULL) {
        if (p1_p->get_dim() == p2_p->get_dim()) {
            return p1_p->distance(p2_p);
        }
    }

    return 1; // arbitrary choice
}

double intracluster_variance(std::unordered_map<Node*, Node*> clusters)
{
    /*!
     * @brief Returns the intracluster variance by computing the centers of 
     * the different clusters first 
     * 
     * @param clusters The different clusters computed, that is, a map between 
     * nodes and their representatives
     * 
     * @return The intracluster variance of these clusters
     * 
     */

    double intracluster_variance = 0.0;

    // firstly, we compute the clusters in the reverse mode, that is, 
    // we take for each representative, all its members
    std::unordered_set<Node*> representatives;
    std::unordered_map<Node*, std::unordered_set<Node*> > members;

    for (auto kv : clusters) {
        Node* node = kv.first;
        Node* rep = kv.second;

        representatives.insert(rep);
        
        if (members.find(rep) == members.end()) {
            members.insert(std::pair<Node*, std::unordered_set<Node*> >(rep, std::unordered_set<Node*>()));
        }

        members.at(rep).insert(node);
    }

    for (Node* rep : representatives) {
        int d = rep->get_point()->get_dim();
        int cluster_size = members.at(rep).size();

        // secondly, we compute centers as in the M-step of kmeans 
        Node* center = new Node(rep->label, new Point());

        for (Node* member : members.at(rep)) {
            for (int j = 0; j < d; j ++) {
                center->get_point()->coords[j] += member->get_point()->coords[j];
            }
        }

        for (int j = 0; j < d; j ++) {
            center->get_point()->coords[j] /= cluster_size;
        }

        // finally, we compute the intracluster variance out of these centers
        double cluster_variance = 0.0;
        for (Node* member : members.at(rep)) {
            cluster_variance += member->get_point()->distance(center->get_point());
        }

        intracluster_variance += cluster_variance;
    }
    intracluster_variance /= clusters.size();

    return intracluster_variance;
}

double silhouette(std::unordered_map<Node*, Node*> clusters)
{
    /*!
     * @brief Computes the silhouette of the clusters given in argument
     *
     * @param clusters A mapping between nodes and their representatives 
     * in clusters
     * 
     * @return The silhouette of these clusters
     */

    int n = clusters.size(); // number of nodes

	double silhouette = 0.0;

    // firstly, we compute the clusters in the reverse mode, that is, 
    // we take for each representative, all its members
    std::unordered_set<Node*> representatives;
    std::unordered_map<Node*, int> clusters_size;
    std::unordered_map<Node*, std::unordered_set<Node*> > members;
    std::unordered_set<Node*> nodes;

    for (auto kv : clusters) {
        Node* node = kv.first;
        Node* rep = kv.second;

        nodes.insert(node);

        representatives.insert(rep);
        if (clusters_size.find(rep) == clusters_size.end()) {
            clusters_size.insert(std::pair<Node*, int>(rep, 0));
        }
        clusters_size.at(rep) += 1;
        
        if (members.find(rep) == members.end()) {
            members.insert(std::pair<Node*, std::unordered_set<Node*> >(rep, std::unordered_set<Node*>()));
        }

        members.at(rep).insert(node);
    }

    int nb_clusters = representatives.size();

	// we compute a and b which are respectively the 
	// average distance to points of the same cluster 
	// and min of average distances to foreign clusters
    std::unordered_map<Node*, double> a;
    std::unordered_map<Node*, double> b;
    std::unordered_map<Node*, double> s;

    for (Node* node : nodes) {
        a.insert(std::pair<Node*, double>(node, 0.0));
        b.insert(std::pair<Node*, double>(node, 0.0));
        s.insert(std::pair<Node*, double>(node, 0.0));
    }

	for (Node* node : nodes){
        std::unordered_map<Node*, double> dist_to_clusters;
        for (Node* rep : representatives) {
            dist_to_clusters.insert(std::pair<Node*, double>(rep, 0.0));
        }

        for (Node* other_node : nodes) {
            if (other_node != node) {
                Node* other_node_cluster = clusters.at(other_node);

                dist_to_clusters.at(other_node_cluster) += std::sqrt(node->dist(other_node));
            }
        }

        double minDist = DBL_MAX;
        for (Node* rep : representatives) {
            dist_to_clusters.at(rep) /= clusters_size.at(rep);

            if (rep != clusters.at(node) && dist_to_clusters.at(rep) < minDist) {
                minDist = dist_to_clusters.at(rep);
            }
        }

        a.at(node) = dist_to_clusters.at(clusters.at(node));
        if (minDist < DBL_MAX) {
            b.at(node) = minDist;
        }
        s.at(node) = (b.at(node) - a.at(node)) / (std::max(a.at(node), b.at(node)));
	}

    for (Node* node : nodes) {
        silhouette += s.at(node);
    }
	silhouette /= n;

	return silhouette;
}

void Node::set_id(node_id_t id_){
    this->id = id_;
}

node_id_t Node::get_id(){
    return this->id;
}
