# This is a sample Python script.
import math

# Press Shift+F10 to execute it or replace it with your code.
# Press Double Shift to search everywhere for classes, files, tool windows, actions, and settings.

epsilon = 0.001


def euclidean_norm(new_mu, old_mu):
    sum = 0
    for i in range(len(new_mu)):
        sum += (new_mu[i] - old_mu[i]) ** 2
    return math.sqrt(sum)


def update_mu(xi_list):
    if len(xi_list) == 0:
        return xi_list
    new_mu = [0 for i in range(len(xi_list[0]))]
    for cord in range(len(xi_list[0])):
        sum = 0
        for xi in xi_list:
            sum += xi[cord]
        # new_mu.append(sum / len(xi_list))
        new_mu[cord] = sum / len(xi_list)
    return new_mu


def argmin(xi, mu_list):
    lst_dec = [0 for i in range(len(mu_list))]
    for j in range(len(mu_list)):
        sum = 0
        for cord in range(len(xi)):
            sum += (xi[cord] - mu_list[j][cord]) ** 2
        lst_dec[j] = (math.sqrt(sum))
    val, idx = min((val, idx) for (idx, val) in enumerate(lst_dec))
    return idx


def k_means(K, filename, max_iter=200):
    file = open(filename)
    mu_list = [[] for i in range(K)]
    d = {}

    for i in range(K):
        mu_list[i] = file.readline().split(",")
        mu_list[i] = [(float(mu)) for mu in mu_list[i]]
        d[i] = []

    file.seek(0, 0)

    for line in file:
        xi = line.split(",")
        xi = [(float(x)) for x in xi]
        closest_mu_index = argmin(xi, mu_list)
        d[closest_mu_index].append(xi)

    delta_max = epsilon
    run_index = 0

    while delta_max >= epsilon and run_index < max_iter:
        run_index += 1
        for mu_index in range(len(mu_list)):
            for xi in d[mu_index]:
                new_mu_index = argmin(xi, mu_list)
                if new_mu_index != mu_index:
                    d[mu_index].remove(xi)
                    d[new_mu_index].append(xi)

        cord_delta_list = []
        for mu_index in range(len(mu_list)):
            new_mu = update_mu(d[mu_index])
            for i in range(len(new_mu)):
                new_mu[i] = float('{:.4f}'.format(new_mu[i]))
            old_mu = mu_list[mu_index]
            cord_delta_list.append(euclidean_norm(new_mu, old_mu))
            mu_list[mu_index] = new_mu

        delta_max = max(cord_delta_list)
        print(delta_max)

    outputfile = open("output.txt", "w")
    for mu in mu_list:
        mu = [(str('{:.4f}'.format(cord))) for cord in mu]
        str1 = ','.join(mu)
        outputfile.write(str1 + '\n')

    return outputfile


# Press the green button in the gutter to run the script.
if __name__ == '__main__':
    file_name = "input_1.txt"
    k_means(3, file_name,600)

# See PyCharm help at https://www.jetbrains.com/help/pycharm/
