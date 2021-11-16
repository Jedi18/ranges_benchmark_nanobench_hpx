import json
import matplotlib.pyplot as plt
import matplotlib.patches as mpatches
import numpy as np
from os import listdir
import os
import csv

SEQ = 0
PAR = 1
RANGES = 2

def generateBenchmark():
    labelToCol = {
        'seq' : 'red',
        'par' : 'blue',
        'ranges' : 'green'
    }

    folder_path = 'C:\\Users\\targe\Documents\\ranges_csv\\ranges_benchmarks\\nanobench'
    folders = listdir(folder_path + '\\csv_files')

    for scan_file in folders:
        fig, ax = plt.subplots()
        bm_data = {}
        bm_data[SEQ] = []
        bm_data[PAR] = []
        bm_data[RANGES] = []

        n = 0

        string_to_enum = {
            'seq_normal':SEQ,
            'par_normal':PAR,
            'range':RANGES
        }

        scan_file_path = folder_path + '\\csv_files\\' + scan_file
        with open(scan_file_path) as jsonfile:
            data = csv.reader(jsonfile, delimiter=";")

            for l in data:
                if l[0] == 'title':
                    continue

                bm_data[string_to_enum[l[1]]].append(float(l[4]))

        n = len(bm_data[SEQ])
        inputSizes = []
        seqTime = []
        parTime = []
        parRelative = []
        rangesRelative = []
        rangeTime = []

        for i in range(0, n):
            inputSizes.append(i + 5)
            seqTime.append(float(bm_data[SEQ][i]))
            parTime.append(float(bm_data[PAR][i]))
            rangeTime.append(float(bm_data[RANGES][i]))
            parRelative.append(float(bm_data[SEQ][i]) / float(bm_data[PAR][i]))
            rangesRelative.append(float(bm_data[SEQ][i]) / float(bm_data[RANGES][i]))
        
        ax.set_xticks(range(5, 32, 1))

        ax.plot(inputSizes, parRelative, color=labelToCol['par'])
        ax.plot(inputSizes, rangesRelative, color=labelToCol['ranges'])
        ax.plot([5, 30], [1, 1], color=labelToCol['seq'])

        ax.set_xlabel('i where the input size is 2^i')
        ax.set_ylabel('Relative speedup to sequential')

        handles = []
        handles.append(mpatches.Patch(color=labelToCol['seq'], label="Seq"))
        handles.append(mpatches.Patch(color=labelToCol['par'], label="Par"))
        handles.append(mpatches.Patch(color=labelToCol['ranges'], label="Ranges"))
        handles.append(mpatches.Patch(color='white', label=""))
        handles.append(mpatches.Patch(color='white', label="L1 Cache - 32 KiB"))
        handles.append(mpatches.Patch(color='white', label="L2 Cache - 512 KiB"))
        handles.append(mpatches.Patch(color='white', label="L3 Cache - 16 MiB"))

        plt.legend(loc="upper left", handles=handles)
        plt.savefig(folder_path + '\\plots\\' + os.path.splitext(scan_file)[0] + '.png')

if __name__ == "__main__":
    generateBenchmark()