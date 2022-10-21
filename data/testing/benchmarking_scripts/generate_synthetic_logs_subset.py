import random
from random import randint
#import numpy as np


def get_activity_from_probability(current_alphabet):
    val = random.uniform(0, 1)
    cumulative = 0

    for key, value in current_alphabet.items():
        cumulative += value
        if cumulative >= val:
            return key
    print('OH NO')


def redistribute_probabilities(dicts, target=None):
    remain_prob_dist = ((1 - dicts[target]) / (len(dicts) - 1)) if target is not None else (1 / (len(dicts) - 1))

    for key, value in dicts.items():
        if key != target:
            dicts[key] = remain_prob_dist


def load_alphabet(alphabet_file='data/alphabets/ABCDE.txt'):
    with open(alphabet_file, 'r', encoding='UTF8', newline='') as f:
        return f.read().splitlines()

    return None


def create_log(alphabet, sample_sizes, sample_lengths, save_log=True):
    logs = []

    for i in range(len(sample_sizes)):
        for m in range(len(sample_lengths)):
            logs.append([])

            if i != 0:
                logs[i * len(sample_lengths) + m] = logs[len(logs) - len(sample_lengths) - 1].copy()

            for n in range(sample_sizes[i] - len(logs[i * len(sample_lengths) + m])):
                trace = []

                for k in range(sample_lengths[m]):
                    alphabet_prob = dict.fromkeys(alphabet, 1 / len(alphabet))
                    activity = get_activity_from_probability(alphabet_prob)
                    trace.append(activity + '\t')

                logs[(i * len(sample_lengths)) + m].append(trace)

            if save_log:
                with open('data/benchmarking/' + str(len(logs[(i * len(sample_lengths)) + m])) + '_' +
                          str(sample_lengths[m]) + '_log.tab', 'w', encoding='UTF8',
                          newline='') as f:
                    for trace in logs[(i * len(sample_lengths)) + m]:
                        f.write('%s\n' % ''.join(trace))
                    f.close()


def main():
    alphabet = load_alphabet('data/alphabets/ABCDE.txt')
    create_log(alphabet, [10000], [10, 100, 1000, 10000])


if __name__ == "__main__":
    main()

