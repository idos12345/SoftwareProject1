import math
import sys

epsilon = 0.001

# euqlide_norm
# output: for every cord (sum of (old_mu[cord]**2 -new_mu[cord])**2 )**0.5
def euclidean_norm(new_mu, old_mu):
    sum = 0
    for i in range(len(new_mu)):
        sum += (new_mu[i] - old_mu[i]) ** 2
    return math.sqrt(sum)

# input: xi list of a certain mu
# output: compute mean of every cord and save it in new mu in the same cord, return new mu
def update_mu(xi_list):
    if len(xi_list) == 0:
        return xi_list
    new_mu = [0 for i in range(len(xi_list[0]))]
    for cord in range(len(xi_list[0])):
        sum = 0
        for xi in xi_list:
            sum += xi[cord]
        new_mu[cord] = sum / len(xi_list)
    return new_mu

# input: xi , mu_list
# return the index of the closest mu of xi
def argmin(xi, mu_list):
    lst_dec = [0 for i in range(len(mu_list))]
    for j in range(len(mu_list)):
        sum = 0
        for cord in range(len(xi)):
            sum += ((xi[cord] - mu_list[j][cord]) ** 2)
        lst_dec[j] = (math.sqrt(sum))
    val, idx = min((val, idx) for (idx, val) in enumerate(lst_dec))
    return idx

#reads first K lines from file and insert them to mu_list
def initial_K_mus(file, K, mu_list,d):
    for i in range(K):
        mu_list[i] = file.readline().split(",")
        mu_list[i] = [(float(mu)) for mu in mu_list[i]]
        d[i] = []

# reads all lines in file and save evrey xi in the value of its closest mu in d
def read_file_and_initial_dict(file, d, mu_list):
    for line in file:
        xi = line.split(",")
        xi = [(float(x)) for x in xi]
        closest_mu_index = argmin(xi, mu_list)
        d[closest_mu_index].append(xi)

# checks if xi is closer to another mu it save the change in changes array
def create_change_array(mu_list,d):
    changes = []  # will save all the changes of xi
    for mu_index in range(len(mu_list)):
        for xi in d[mu_index]:
            new_mu_index = argmin(xi, mu_list)  # compute the new mu for xi
            if new_mu_index != mu_index:  # if xi changed it's mu
                changes.append((mu_index, new_mu_index, xi))  # save the change
    return changes

# implement all the changes
def implement_changes(changes, d):
    for (s, t, xi) in changes:
            d[s].remove(xi)
            d[t].append(xi)

# update evrey mu by its's xi and clculate deltas
def update_mu_list_and_create_deltas_list(mu_list,d):
    cord_delta_list = []
    for mu_index in range(len(mu_list)):
        new_mu = update_mu(d[mu_index])
        for i in range(len(new_mu)):
            new_mu[i] = float('{:.4f}'.format(new_mu[i]))  # keeping only 4 numbers after point
        old_mu = mu_list[mu_index]
        cord_delta_list.append(euclidean_norm(new_mu, old_mu))  # compute and save delta between new and ols mu
        mu_list[mu_index] = new_mu  # update mus list
    return cord_delta_list
    
def write_to_output_txt(mu_list, output_filename):
    output_filename = open(output_filename, "w")  # create output file and write all mus into it
    for mu in mu_list:
        mu = [(str('{:.4f}'.format(cord))) for cord in mu]
        str1 = ','.join(mu)
        output_filename.write(str1 + '\n')

def kmeans(K, input_filename, output_filename, max_iter=200):
    file = open(input_filename)
    mu_list = [[] for i in range(K)]
    d = {}

    initial_K_mus(file,K,mu_list,d)
    file.seek(0, 0)  # return to the beginning of the file
    read_file_and_initial_dict(file,d,mu_list)
    
    delta_max = epsilon
    run_index = 0
    while delta_max >= epsilon and run_index < max_iter:
        run_index += 1
        changes = create_change_array(mu_list,d)
        implement_changes(changes, d)
        cord_delta_list = update_mu_list_and_create_deltas_list(mu_list,d)
        delta_max = max(cord_delta_list)  # find the max delta

    write_to_output_txt(mu_list,output_filename)
    return output_filename

def submit_args():
    if len(sys.argv) != 4 and len(sys.argv) != 5:
        print("Invalid Input!")
        return 0
    try:
        k = int(sys.argv[1])
        if len(sys.argv) == 5:

            max_iter = int(sys.argv[2])
            input_file_name = sys.argv[3]
            output_file_name = sys.argv[4]
        else:
            max_iter = 200
            input_file_name = sys.argv[2]
            output_file_name = sys.argv[3]

        if max_iter <= 0 or k <= 0:
            return 0

        f_input = open(input_file_name)
        f_output = open(output_file_name,'w')
        f_input.close()
        f_output.close()

    except OSError:
        print("Invalid Input!")
        return 0

    return k, max_iter, input_file_name, output_file_name

def main():
    args = submit_args()
    if args == 0:
        return 0
    k, max_iter, input_file_name, output_file_name = args
    kmeans(k, input_file_name,output_file_name, max_iter)

    return 1

if __name__ == '__main__':
    main()