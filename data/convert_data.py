"""
The only purpose of this file is to treat datasets that have been 
downloaded from the internet. Since there is no general way to convert 
csv data files to txt easy-to-read files for k-NN, we assume that 
the datasets to be converted are relatively simple. That is, these 
files contain a header line for all the data details, then for each 
variable there can be a float value, a TRUE/FALSE value (with different 
capitalization) or a variable to be discarded. Finally, it handles the lack of 
values (NaN) by inserting the mean of all known values in other lines,
or zero if there is none. 
"""

import sys 
import math
import numpy as np
from sklearn.preprocessing import scale

true_variants = {"TRUE", "TRUE\n", "true", "true\n", "True", "True\n"}
false_variants = {"FALSE", "FALSE\n", "false", "false\n", "False", "False\n"}
nan_variants = {"NA", "NA\n", "na", "na\n", "NaN", "NaN\n", "nan", "nan\n"}

def read_file(file_to_read, sep=",", except_cols=set()):
    """Reads a file from its name. It is expected to be in the data 
    folder of the current project. 

    Args:
        file_to_read (str)  : the name of the file, including extension
        sep (str)           : the separator between columns in the dataset
        except_cols (tuple) : a list of columns that have to be ejected
            from the dataset

    Returns:
        None
    """

    with open(file_to_read, "r", encoding="utf-8") as f:

        header = f.readline().split(sep=sep)
        variables = "The variables kept from this file are: "
        for k in range(len(header)):
            if k not in except_cols:
                variables += header[k] + " "
        print(variables)

        for line in f:

            values = line.split(sep=sep)

            kept_values = [0 for i in range(len(values) - len(except_cols))]
            current_col = 0
            for i in range(len(values)):

                if i not in except_cols:

                    # treatment of NaN values
                    if values[i] in nan_variants:
                        kept_values[current_col] = float("NaN")

                    # treatment of TRUE/FALSE 
                    elif values[i] in true_variants:
                        kept_values[current_col] = 1
                    elif values[i] in false_variants:
                        kept_values[current_col] = 0

                    else:
                        kept_values[current_col] = float(values[i])
            
                    current_col += 1

            for k in range(len(kept_values)):
                print(kept_values[k], end= " ")
            print("")

    return 

def write_file(data, file_to_write):
    """Writes a file in a txt format that can be easily read
    by any C++ file we have in this project (for instance, we 
    use this for kmeans and mst clustering)
    Warning: it will overwrite any existing file with the same name

    Args:
        data (list)         : a list of list, where each contained list 
            is a row of the output file, containing only float
            values
        file_to_write (str) : name to be giver to the new txt file

    Returns:
        None
    """

    n = len(data)
    m = len(data[0])

    with open(file_to_write, "w", encoding="utf-8") as f:

        for i in range(n):

            line = ""

            for j in range(m-1):
                line += str(data[i][j]) + " "
            line += str(data[i][m-1])

            f.write(line + "\n")

    return

def convert_file(file_to_read, sep=",", except_cols=set(), do_scale=False, nb_points=100):
    """Converts a file which respects the requirements from detailed in 
    the header of this file to a txt easy-to-read file for C++ algorithms

    Args:
        file_to_read (str)  : the name of the file, including extension
        sep (str)           : the separator between columns in the dataset
        except_cols (tuple) : a list of columns that have to be ejected
            from the dataset
        do_scale (bool)     : whether the data has to be scaled before being 
            written in the new file
        nb_points (int)     : the number of points to keep from the initial 
            dataset for the new one, the indices being chosen randomly
    
    Returns:
        None
    """

    data = []
    file_to_write = ""

    # Reading the file and converting simple values to float (keeping NaN values)
    with open(file_to_read, "r", encoding="utf-8") as f:

        header = f.readline().split(sep=sep)
        variables = "The variables kept from this file are: "
        for k in range(len(header)):
            if k not in except_cols:
                variables += header[k] + " "
        print(variables)

        for line in f:

            values = line.split(sep=sep)

            kept_values = [0 for i in range(len(values) - len(except_cols))]
            current_col = 0
            for i in range(len(values)):

                if i not in except_cols:

                    # treatment of NaN values
                    if values[i] in nan_variants:
                        kept_values[current_col] = float("NaN")

                    # treatment of TRUE/FALSE 
                    elif values[i] in true_variants:
                        kept_values[current_col] = 1
                    elif values[i] in false_variants:
                        kept_values[current_col] = 0

                    else:
                        kept_values[current_col] = float(values[i])
            
                    current_col += 1

            data.append(kept_values) 

    # Treating NaN values using the mean of known values 
    n = len(data)
    m = len(data[0])
    for j in range(m):

        missing_indices = []
        known_values = 0
        mean = 0
        for i in range(n):
            if math.isnan(data[i][j]):
                missing_indices.append(i)
            else:
                mean += data[i][j]
                known_values += 1
        mean = 0 if known_values <= 0 else mean/known_values

        for i in range(n):
            if math.isnan(data[i][j]):
                data[i][j] = mean

    # Scaling step
    if do_scale:
        data = scale(data)
        file_to_write += "scaled_"

    # Selecting a certain part of the data 
    indices = [] 
    if nb_points >= n:
        indices = [i for i in range(n)]
    else:
        indices = np.random.choice(n, nb_points)

    data = [data[idx] for idx in indices]
    file_to_write += "n" + str(nb_points) + "_"

    # Writing a brand new file !
    file_to_write += file_to_read.split(".")[0] + ".txt"
    write_file(data, file_to_write)
    print("Data has been written in the file: " + file_to_write)

    return 

### Usage of this file via command prompt

nb_args = len(sys.argv)
if nb_args >= 2:
	file_to_read = sys.argv[1]
else:
	print("""Syntax: python {} <file_to_read> [sep] ["except_cols1;except_cols2;..."] [do_scale:0/1] [nb_points]""".format(sys.argv[0]))
	exit(0)

sep = ","
except_cols = set()
do_scale = False
nb_points = 100

# Selection of separator (format: ",")
if nb_args >= 3:
    sep = sys.argv[2]

# Selection of columns to except (format: "x;y;z")
if nb_args >= 4:
    cols_to_except = sys.argv[3].split(";")
    for col in cols_to_except:
        except_cols.add(int(col))

# Selection of scaling (format: "0/1")
if nb_args >= 5:
    do_scale = bool(int(sys.argv[4]))

# Selection of the number of points 
if nb_args >= 6:
    nb_points = int(sys.argv[5])

convert_file(file_to_read, sep=sep, except_cols=except_cols, do_scale=do_scale, nb_points=nb_points)
