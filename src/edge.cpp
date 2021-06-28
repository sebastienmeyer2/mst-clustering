#include "edge.hpp"

#include <sstream>

Edge::Edge(Node* p1, Node* p2){
    /*!
     * @brief Constructeur de l'arrête
     * 
     * @param p1 Le premier noeud constituant l'arrête
     * @param p2 Le second noeud constituant l'arrête
     * 
     * @note Quand aucun poids n'est spécifié et que les noeuds ne portent pas d'information, 
     * on considère qu'il vaut 1. 
     * 
     * @note Voir \ref Edge::Edge(Node* p1, Node* p2, edge_weight_t weight) pour d'autres remarques.
     * 
     */

    this->p1 = p1;
    this->p2 = p2;
    this->weight = p1->dist(p2);
}

Edge::Edge(Node* p1, Node* p2, edge_weight_t weight){
    /*!
     * @brief Constructeur de l'arrête avec poids donné
     * 
     * @param p1 Le premier noeud constituant l'arrête
     * @param p2 Le second noeud constituant l'arrête
     * @param weight Le poids de l'arrête
     * 
     * @note Comme on ne considère que des graphes non dirigés, l'ordre de p1 et p2 n'a pas d'importance
     * 
     */

    this->p1 = p1;
    this->p2 = p2;
    this->weight = weight;
}

Node* Edge::other_node(Node* p){
    /*!
     * @brief Renvoie le noeud de l'arrête différent de \p p
     * 
     * @param p Le noeud de l'arrête qu'on connaît
     * 
     * @return L'autre noeud, ou nullptr si \p p n'appartient pas à l'arrête
     *  
     */

    if(this->p1 != p && this->p2 != p)
        return nullptr;
    
    return this->p1 == p ? this->p2 : this->p1;
}

bool Edge::operator<(Edge const& other) const 
{
    return this->weight < other.weight;
}

