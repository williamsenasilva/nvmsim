#! /usr/bin/python3
# -*- coding: UTF-8 -*-

import glob
import sys
import os.path
import numpy as np
import pandas as pd
import matplotlib as mpl
import matplotlib.pyplot as plt
import random

#benchmark_names = ["astar","bwaves","bzip2","gamess","gcc","gobmk","h264ref","hmmer","libquantum","mcf","omnetpp","perlbench","sjeng","specrand","xalancbmk"]
benchmark_names = ["astar","gamess","gobmk","h264ref","hmmer","libquantum","mcf","omnetpp","sjeng","specrand","xalancbmk"]
memories = ["2D_DRAM","3D_DRAM","PCM","RRAM","STTRAM"]
confidence_percentage = 95
rearrangements_size = 100000

class Benchmark:
    def __init__(self, name):
        self.name = name
        self.path = ""
        self.sim_dot_out_2dram_files = []
        self.sim_dot_out_3dram_files = []
        self.sim_dot_out_pcm_files = []
        self.sim_dot_out_rram_files = []
        self.sim_dot_out_sttram_files = []

class Simulation:
    def __init__(self, path):
        self.path = path
        self.benchmarks = []
        self.dataframe = []

    def get_benchmarks_paths(self):
        benchmark_paths = glob.glob('{}/*'.format(self.path))
        for benchmark_path in benchmark_paths:
            benchmark_name = benchmark_path.rsplit('/', 1)[-1].split(".",1)[1]
            if benchmark_name in benchmark_names:
                benchmark = Benchmark(benchmark_name)
                benchmark.path = benchmark_path
                self.benchmarks.append(benchmark)

    def get_sim_dot_outs_and_results(self):
        for benchmark in self.benchmarks:
            test_paths = glob.glob('{}/*'.format(benchmark.path))
            for test_path in test_paths:
                sim_dot_out_path = test_path + "/sim.out"
                if os.path.isfile(sim_dot_out_path) and sim_dot_out_path.find("test-1") > -1:
                    if sim_dot_out_path.find("2D_DRAM") > -1:
                        with open(sim_dot_out_path, "r") as sim_out_file:
                            measure_latency = 0
                            measure_time = 0
                            content = sim_out_file.readlines()
                            for line in content:
                                if "Time (ns)" in line:
                                    measure_time = line.split("|",1)[1].replace(" ","").replace("\n","")
                                elif "average dram access latency (ns)" in line:
                                    measure_latency = line.split("|",1)[1].replace(" ","").replace("\n","")
                            data = (sim_dot_out_path,measure_time, measure_latency)
                            benchmark.sim_dot_out_2dram_files.append(data)
                    elif sim_dot_out_path.find("3D_DRAM") > -1:
                        with open(sim_dot_out_path, "r") as sim_out_file:
                            measure_latency = 0
                            measure_time = 0
                            content = sim_out_file.readlines()
                            for line in content:
                                if "Time (ns)" in line:
                                    measure_time = line.split("|",1)[1].replace(" ","").replace("\n","")
                                elif "average dram access latency (ns)" in line:
                                    measure_latency = line.split("|",1)[1].replace(" ","").replace("\n","")
                            data = (sim_dot_out_path,measure_time, measure_latency)
                            benchmark.sim_dot_out_3dram_files.append(data)
                    elif sim_dot_out_path.find("PCM") > -1:
                        with open(sim_dot_out_path, "r") as sim_out_file:
                            measure_latency = 0
                            measure_time = 0
                            content = sim_out_file.readlines()
                            for line in content:
                                if "Time (ns)" in line:
                                    measure_time = line.split("|",1)[1].replace(" ","").replace("\n","")
                                elif "average dram access latency (ns)" in line:
                                    measure_latency = line.split("|",1)[1].replace(" ","").replace("\n","")
                            data = (sim_dot_out_path,measure_time, measure_latency)
                            benchmark.sim_dot_out_pcm_files.append(data)
                    elif sim_dot_out_path.find("RRAM") > -1:
                        with open(sim_dot_out_path, "r") as sim_out_file:
                            measure_latency = 0
                            measure_time = 0
                            content = sim_out_file.readlines()
                            for line in content:
                                if "Time (ns)" in line:
                                    measure_time = line.split("|",1)[1].replace(" ","").replace("\n","")
                                elif "average dram access latency (ns)" in line:
                                    measure_latency = line.split("|",1)[1].replace(" ","").replace("\n","")
                            data = (sim_dot_out_path,measure_time, measure_latency)
                            benchmark.sim_dot_out_rram_files.append(data)
                    elif sim_dot_out_path.find("STTRAM") > -1:
                        with open(sim_dot_out_path, "r") as sim_out_file:
                            measure_latency = 0
                            measure_time = 0
                            content = sim_out_file.readlines()
                            for line in content:
                                if "Time (ns)" in line:
                                    measure_time = line.split("|",1)[1].replace(" ","").replace("\n","")
                                elif "average dram access latency (ns)" in line:
                                    measure_latency = line.split("|",1)[1].replace(" ","").replace("\n","")
                            data = (sim_dot_out_path,measure_time, measure_latency)
                            benchmark.sim_dot_out_sttram_files.append(data)
    
    def make_dataframe(self):
        for benchmark in self.benchmarks:
            times = []
            latencies = []
            time_mean = 0
            latency_mean = 0
            meanings = len(benchmark.sim_dot_out_2dram_files)
            print("{}|{} - {} measures".format(benchmark.name,"2DRAM", meanings))
            for sim_dot_out_file in benchmark.sim_dot_out_2dram_files:
                filename, time_in_ns, latency_in_ns = (sim_dot_out_file)
                times.append(int(time_in_ns))
                time_mean += int(time_in_ns)
                latencies.append(float(latency_in_ns))
                latency_mean += float(latency_in_ns)
                print("  time: {} | latency: {} | file: {}".format(time_in_ns, latency_in_ns, filename.split("simulations/",1)[1].replace(" ","")))
            time_mean = time_mean / meanings
            time_err = calc_ci_using_bootstrap(times)
            latency_mean = (latency_mean / meanings)
            latency_err = calc_ci_using_bootstrap(latencies)
            line = [benchmark.name, "2D_RAM", time_mean, latency_mean, time_err, latency_err]
            self.dataframe.append(line)
            print("  time list: {}\n  ltcy list: {}".format(times, latencies))
            print("  time mean: {}\n  ltcy mean: {}".format(time_mean, latency_mean))
            print("  time err : {}\n  ltcy err : {}".format(time_err, latency_err))
            print()

            times = []
            latencies = []
            time_mean = 0
            latency_mean = 0
            meanings = len(benchmark.sim_dot_out_3dram_files)
            print("{}|{} - {} measures".format(benchmark.name,"3DRAM", meanings))
            for sim_dot_out_file in benchmark.sim_dot_out_3dram_files:
                filename, time_in_ns, latency_in_ns = (sim_dot_out_file)
                times.append(int(time_in_ns))
                time_mean += int(time_in_ns)
                latencies.append(float(latency_in_ns))
                latency_mean += float(latency_in_ns)
                print("  time: {} | latency: {} | file: {}".format(time_in_ns, latency_in_ns, filename.split("simulations/",1)[1].replace(" ","")))
            time_mean = time_mean / meanings
            time_err = calc_ci_using_bootstrap(times)
            latency_mean = (latency_mean / meanings)
            latency_err = calc_ci_using_bootstrap(latencies)
            line = [benchmark.name, "3D_RAM", time_mean, latency_mean, time_err, latency_err]
            self.dataframe.append(line)
            print("  time list: {}\n  ltcy list: {}".format(times, latencies))
            print("  time mean: {}\n  ltcy mean: {}".format(time_mean, latency_mean))
            print("  time err : {}\n  ltcy err : {}".format(time_err, latency_err))
            print()

            times = []
            latencies = []
            time_mean = 0
            latency_mean = 0
            meanings = len(benchmark.sim_dot_out_pcm_files)
            print("{}|{} - {} measures".format(benchmark.name,"PCM", meanings))
            for sim_dot_out_file in benchmark.sim_dot_out_pcm_files:
                filename, time_in_ns, latency_in_ns = (sim_dot_out_file)
                times.append(int(time_in_ns))
                time_mean += int(time_in_ns)
                latencies.append(float(latency_in_ns))
                latency_mean += float(latency_in_ns)
                print("  time: {} | latency: {} | file: {}".format(time_in_ns, latency_in_ns, filename.split("simulations/",1)[1].replace(" ","")))
            time_mean = time_mean / meanings
            time_err = calc_ci_using_bootstrap(times)
            latency_mean = (latency_mean / meanings)
            latency_err = calc_ci_using_bootstrap(latencies)
            line = [benchmark.name, "PCM", time_mean, latency_mean, time_err, latency_err]
            self.dataframe.append(line)
            print("  time list: {}\n  ltcy list: {}".format(times, latencies))
            print("  time mean: {}\n  ltcy mean: {}".format(time_mean, latency_mean))
            print("  time err : {}\n  ltcy err : {}".format(time_err, latency_err))
            print()

            times = []
            latencies = []
            time_mean = 0
            latency_mean = 0
            meanings = len(benchmark.sim_dot_out_rram_files)
            print("{}|{} - {} measures".format(benchmark.name,"RRAM", meanings))
            for sim_dot_out_file in benchmark.sim_dot_out_rram_files:
                filename, time_in_ns, latency_in_ns = (sim_dot_out_file)
                times.append(int(time_in_ns))
                time_mean += int(time_in_ns)
                latencies.append(float(latency_in_ns))
                latency_mean += float(latency_in_ns)
                print("  time: {} | latency: {} | file: {}".format(time_in_ns, latency_in_ns, filename.split("simulations/",1)[1].replace(" ","")))
            time_mean = time_mean / meanings
            time_err = calc_ci_using_bootstrap(times)
            latency_mean = (latency_mean / meanings)
            latency_err = calc_ci_using_bootstrap(latencies)
            line = [benchmark.name, "RRAM", time_mean, latency_mean, time_err, latency_err]
            self.dataframe.append(line)
            print("  time list: {}\n  ltcy list: {}".format(times, latencies))
            print("  time mean: {}\n  ltcy mean: {}".format(time_mean, latency_mean))
            print("  time err : {}\n  ltcy err : {}".format(time_err, latency_err))
            print()

            times = []
            latencies = []
            time_mean = 0
            latency_mean = 0
            meanings = len(benchmark.sim_dot_out_sttram_files)
            print("{}|{} - {} measures".format(benchmark.name,"STTRAM", meanings))
            for sim_dot_out_file in benchmark.sim_dot_out_sttram_files:
                filename, time_in_ns, latency_in_ns = (sim_dot_out_file)
                times.append(int(time_in_ns))
                time_mean += int(time_in_ns)
                latencies.append(float(latency_in_ns))
                latency_mean += float(latency_in_ns)
                print("  time: {} | latency: {} | file: {}".format(time_in_ns, latency_in_ns, filename.split("simulations/",1)[1].replace(" ","")))
            time_mean = time_mean / meanings
            time_err = calc_ci_using_bootstrap(times)
            latency_mean = (latency_mean / meanings)
            latency_err = calc_ci_using_bootstrap(latencies)
            line = [benchmark.name, "STTRAM", time_mean, latency_mean, time_err, latency_err]
            self.dataframe.append(line)
            print("  time list: {}\n  ltcy list: {}".format(times, latencies))
            print("  time mean: {}\n  ltcy mean: {}".format(time_mean, latency_mean))
            print("  time err : {}\n  ltcy err : {}".format(time_err, latency_err))
            print()
    
    def normalize_time_values(self):
        for index, line in enumerate(self.dataframe):
            if self.dataframe[index][1] == "2D_RAM":
                normalize_value = self.dataframe[index][2]
                self.dataframe[index][2] = self.dataframe[index][2] / normalize_value
                self.dataframe[index+1][2] = self.dataframe[index+1][2] / normalize_value
                self.dataframe[index+2][2] = self.dataframe[index+2][2] / normalize_value
                self.dataframe[index+3][2] = self.dataframe[index+3][2] / normalize_value
                self.dataframe[index+4][2] = self.dataframe[index+4][2] / normalize_value
        print(self.dataframe)

def generate_latency_graph(simulation):
    df = pd.DataFrame(simulation.dataframe, columns=["benchmark","memory", "time", "latency", "time-error", "latency-error"])
    
    memory_2dram_means = df.loc[df['memory'] == '2D_RAM']["latency"].tolist()
    memory_2dram_error = df.loc[df['memory'] == '2D_RAM']["latency-error"].tolist()
    memory_3dram_means = df.loc[df['memory'] == '3D_RAM']["latency"].tolist()
    memory_3dram_error = df.loc[df['memory'] == '3D_RAM']["latency-error"].tolist()
    memory_pcm_means = df.loc[df['memory'] == 'PCM']["latency"].tolist()
    memory_pcm_error = df.loc[df['memory'] == 'PCM']["latency-error"].tolist()
    memory_rram_means = df.loc[df['memory'] == 'RRAM']["latency"].tolist()
    memory_rram_error = df.loc[df['memory'] == 'RRAM']["latency-error"].tolist()
    memory_sttram_means = df.loc[df['memory'] == 'STTRAM']["latency"].tolist()
    memory_sttram_error = df.loc[df['memory'] == 'STTRAM']["latency-error"].tolist()
    
    # pontos do eixo x
    x = np.arange(len(benchmark_names))
    
    # seta a largura das barras
    width = 0.15

    fig, ax = plt.subplots()

    bars_1 = ax.bar(x - width*2, memory_2dram_means, width, yerr=memory_2dram_error, label="2D_RAM", color="#0d6efd", edgecolor="white")
    bars_2 = ax.bar(x - width*1, memory_3dram_means, width, yerr=memory_3dram_error, label="3D_RAM", color="#dc3545", edgecolor="white")
    bars_3 = ax.bar(x          , memory_pcm_means, width, yerr=memory_pcm_error, label="PCM", color="#ffc107", edgecolor="white")
    bars_4 = ax.bar(x + width*1, memory_rram_means, width, yerr=memory_rram_error, label="RRAM", color="#20c997", edgecolor="white")
    bars_5 = ax.bar(x + width*2, memory_sttram_means, width, yerr=memory_sttram_error, label="STTRAM", color="#fd7e14", edgecolor="white")

    # insere as legendas formatados
    ax.set_ylabel('LATÊNCIA (ns)', weight='bold')
    ax.set_title('Latência média de execução', weight='bold')
    ax.set(ylim=(0, 500))
    ax.set_xticks(x)
    ax.set_xticklabels(benchmark_names)
    ax.legend(title='', loc='best')

    # insere os valores no top odas barras
    ax.bar_label(bars_1, fmt='%.2f', padding=2, rotation=75, size=7.5)
    ax.bar_label(bars_2, fmt='%.2f', padding=2, rotation=75, size=7.5)
    ax.bar_label(bars_3, fmt='%.2f', padding=2, rotation=75, size=7.5)
    ax.bar_label(bars_4, fmt='%.2f', padding=2, rotation=75, size=7.5)
    ax.bar_label(bars_5, fmt='%.2f', padding=2, rotation=75, size=7.5)

    # cria o grid apenas em cima do eixo y com linhas cinzas e finas
    plt.grid(axis="y", color='gray', linewidth=0.075)

    # remove as linhas do topo e da direita do quadro do gráfico
    ax.spines['right'].set_visible(False)
    ax.spines['top'].set_visible(False)

    # maximiza a imagem
    mng = plt.get_current_fig_manager()
    mng.resize(*mng.window.maxsize())

    plt.show()


def generate_time_graph(simulation):
    df = pd.DataFrame(simulation.dataframe, columns=["benchmark", "memory", "time", "latency", "time-error", "latency-error"])
    
    memory_2dram_means = df.loc[df['memory'] == '2D_RAM']["time"].tolist()
    memory_2dram_error = df.loc[df['memory'] == '2D_RAM']["time-error"].tolist()
    memory_3dram_means = df.loc[df['memory'] == '3D_RAM']["time"].tolist()
    memory_3dram_error = df.loc[df['memory'] == '3D_RAM']["time-error"].tolist()
    memory_pcm_means = df.loc[df['memory'] == 'PCM']["time"].tolist()
    memory_pcm_error = df.loc[df['memory'] == 'PCM']["time-error"].tolist()
    memory_rram_means = df.loc[df['memory'] == 'RRAM']["time"].tolist()
    memory_rram_error = df.loc[df['memory'] == 'RRAM']["time-error"].tolist()
    memory_sttram_means = df.loc[df['memory'] == 'STTRAM']["time"].tolist()
    memory_sttram_error = df.loc[df['memory'] == 'STTRAM']["time-error"].tolist()
    
    # pontos do eixo x
    x = np.arange(len(benchmark_names))
    
    # seta a largura das barras
    width = 0.15

    fig, ax = plt.subplots()

    bars_1 = ax.bar(x - width*2, memory_2dram_means, width, yerr=memory_2dram_error, label="2D_RAM", color="#0d6efd", edgecolor="white")
    bars_2 = ax.bar(x - width*1, memory_3dram_means, width, yerr=memory_3dram_error, label="3D_RAM", color="#dc3545", edgecolor="white")
    bars_3 = ax.bar(x          , memory_pcm_means, width, yerr=memory_pcm_error, label="PCM", color="#ffc107", edgecolor="white")
    bars_4 = ax.bar(x + width*1, memory_rram_means, width, yerr=memory_rram_error, label="RRAM", color="#20c997", edgecolor="white")
    bars_5 = ax.bar(x + width*2, memory_sttram_means, width, yerr=memory_sttram_error, label="STTRAM", color="#fd7e14", edgecolor="white")

    # insere as legendas formatados
    ax.set_title("Tempo médio de execução", weight='bold')
    ax.set_ylabel('TEMPO (ns) / 2DRAM', weight='bold')
    ax.set(ylim=(0, 5.5))
    ax.set_xticks(x)
    ax.set_xticklabels(benchmark_names)
    ax.legend(title='', loc='best')

    # insere os valores no top odas barras
    ax.bar_label(bars_1, fmt='%.2f', padding=2, rotation=75, size=7.5)
    ax.bar_label(bars_2, fmt='%.2f', padding=2, rotation=75, size=7.5)
    ax.bar_label(bars_3, fmt='%.2f', padding=2, rotation=75, size=7.5)
    ax.bar_label(bars_4, fmt='%.2f', padding=2, rotation=75, size=7.5)
    ax.bar_label(bars_5, fmt='%.2f', padding=2, rotation=75, size=7.5)

    # cria o grid apenas em cima do eixo y com linhas cinzas e finas
    plt.grid(axis="y", color='gray', linewidth=0.075)

    # remove as linhas do topo e da direita do quadro do gráfico
    ax.spines['right'].set_visible(False)
    ax.spines['top'].set_visible(False)

    # maximiza a imagem
    mng = plt.get_current_fig_manager()
    mng.resize(*mng.window.maxsize())

    plt.show()

def calc_ci_using_bootstrap(samples):
    rearrangements = [[0 for x in range(len(samples))] for y in range(rearrangements_size)] 
    rearrangements_means = [0 for x in range(rearrangements_size)]

    for i in range(rearrangements_size):
        mean = 0
        for k in range(len(samples)):
            rearrangements[i][k] = samples[random.randrange(len(samples))]
            mean += rearrangements[i][k]
        mean = mean / len(samples)
        rearrangements_means[i] = mean
    
    rearrangements_means = sorted(rearrangements_means)
    value_min = value_max = -1
    percentage_min = (100 - confidence_percentage) / 2
    percentage_max = confidence_percentage + percentage_min
    for index, mean in enumerate(rearrangements_means):
        percentage = 100 * index / rearrangements_size
        if value_min < 0 and percentage == percentage_min:
            value_min = mean
        if value_max <0 and percentage == percentage_max:
            value_max = mean
        #print("index: {} | mean: {} | %: {}".format(index,mean, percentage))
    err = value_max - value_min
    #print("min(%): {}({}) | max(%): {}({}) | err: {}".format(value_min, percentage_min, value_max, percentage_max, err))
    return err

if __name__ == '__main__':
    simulations = []
    if len(sys.argv) < 2:
        print("[NVMSIM][ERROR] Missing arg to folder to simulations")
        exit()
    
    # carrega as simulacoes (simulation-1, simulation-2, ..., simulation-n) 
    simulations_path = sys.argv[1]
    simulations_paths = glob.glob('{}/*'.format(simulations_path))
    for simulation_path in simulations_paths:
        name = simulation_path.rsplit('/', 1)[-1] 
        simulation = Simulation(name)
        simulation.path = simulation_path
        simulation.get_benchmarks_paths()
        simulation.get_sim_dot_outs_and_results()
        simulations.append(simulation)

    # unifica todas as simulacoes em apenas uma lista
    united_simulation = Simulation("united")
    for benchmark_name in benchmark_names:
        benchmark = Benchmark(benchmark_name)
        for simulation in simulations:
            for benchmark_aux in simulation.benchmarks:
                if benchmark_name == benchmark_aux.name:
                    benchmark.sim_dot_out_2dram_files += benchmark_aux.sim_dot_out_2dram_files
                    benchmark.sim_dot_out_3dram_files += benchmark_aux.sim_dot_out_3dram_files
                    benchmark.sim_dot_out_pcm_files += benchmark_aux.sim_dot_out_pcm_files
                    benchmark.sim_dot_out_rram_files += benchmark_aux.sim_dot_out_rram_files
                    benchmark.sim_dot_out_sttram_files += benchmark_aux.sim_dot_out_sttram_files
        united_simulation.benchmarks.append(benchmark)
    
    # cria o dataframe completo - medias das 10 simulacoes mais erros calculados via bootstrap
    united_simulation.make_dataframe()

    # normaliza os valores do time em relacao aos da 2DRAM 
    united_simulation.normalize_time_values()

    # gera os gráficos
    generate_time_graph(united_simulation)
    generate_latency_graph(united_simulation)
