#! /usr/bin/python3
# -*- coding: UTF-8 -*-

import glob
import sys
import os.path
import numpy as np
import pandas as pd
import matplotlib as mpl
import matplotlib.pyplot as plt
import seaborn as sns

benchmark_names = ["astar","bwaves","bzip2","gamess","gcc","gobmk","h264ref","hmmer","libquantum","mcf","omnetpp","perlbench","sjeng","specrand","xalancbmk"]
memories = ["2D_DRAM","3D_DRAM","PCM","RRAM","STTRAM"]

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
            time_mean = 0
            latency_mean = 0
            meanings = len(benchmark.sim_dot_out_2dram_files)
            print("{}|{} - {} measures".format(benchmark.name,"2DRAM", meanings))
            for sim_dot_out_file in benchmark.sim_dot_out_2dram_files:
                filename, time_in_ns, latency_in_ns = (sim_dot_out_file)
                time_mean += int(time_in_ns)
                latency_mean += float(latency_in_ns)
                print("  time: {} | latency: {} | file: {}".format(time_in_ns, latency_in_ns, filename.split("simulations/",1)[1].replace(" ","")))
            time_mean = time_mean / meanings
            latency_mean = latency_mean / meanings
            print("  mean time: {}\n  mean ltcy: {}\n".format(time_mean, round(latency_mean,2)))
            line = [benchmark.name, "2D_RAM", time_mean, latency_mean]
            self.dataframe.append(line)

            time_mean = 0
            latency_mean = 0
            meanings = len(benchmark.sim_dot_out_3dram_files)
            print("{}|{} - {} measures".format(benchmark.name,"3DRAM", meanings))
            for sim_dot_out_file in benchmark.sim_dot_out_3dram_files:
                filename, time_in_ns, latency_in_ns = (sim_dot_out_file)
                time_mean += int(time_in_ns)
                latency_mean += float(latency_in_ns)
                print("  time: {} | latency: {} | file: {}".format(time_in_ns, latency_in_ns, filename.split("simulations/",1)[1].replace(" ","")))
            time_mean = time_mean / meanings
            latency_mean = latency_mean / meanings
            print("  mean time: {}\n  mean ltcy: {}\n".format(time_mean, round(latency_mean,2)))
            line = [benchmark.name, "3D_RAM", time_mean, latency_mean]
            self.dataframe.append(line)

            time_mean = 0
            latency_mean = 0
            meanings = len(benchmark.sim_dot_out_pcm_files)
            print("{}|{} - {} measures".format(benchmark.name,"PCM", meanings))
            for sim_dot_out_file in benchmark.sim_dot_out_pcm_files:
                filename, time_in_ns, latency_in_ns = (sim_dot_out_file)
                time_mean += int(time_in_ns)
                latency_mean += float(latency_in_ns)
                print("  time: {} | latency: {} | file: {}".format(time_in_ns, latency_in_ns, filename.split("simulations/",1)[1].replace(" ","")))
            time_mean = time_mean / meanings
            latency_mean = latency_mean / meanings
            print("  mean time: {}\n  mean ltcy: {}\n".format(time_mean, round(latency_mean,2)))
            line = [benchmark.name, "PCM", time_mean, latency_mean]
            self.dataframe.append(line)

            time_mean = 0
            latency_mean = 0
            meanings = len(benchmark.sim_dot_out_rram_files)
            print("{}|{} - {} measures".format(benchmark.name,"RRAM", meanings))
            for sim_dot_out_file in benchmark.sim_dot_out_rram_files:
                filename, time_in_ns, latency_in_ns = (sim_dot_out_file)
                time_mean += int(time_in_ns)
                latency_mean += float(latency_in_ns)
                print("  time: {} | latency: {} | file: {}".format(time_in_ns, latency_in_ns, filename.split("simulations/",1)[1].replace(" ","")))
            time_mean = time_mean / meanings
            latency_mean = latency_mean / meanings
            print("  mean time: {}\n  mean ltcy: {}\n".format(time_mean, round(latency_mean,2)))
            line = [benchmark.name, "RRAM", time_mean, latency_mean]
            self.dataframe.append(line)

            time_mean = 0
            latency_mean = 0
            meanings = len(benchmark.sim_dot_out_sttram_files)
            print("{}|{} - {} measures".format(benchmark.name,"STTRAM", meanings))
            for sim_dot_out_file in benchmark.sim_dot_out_sttram_files:
                filename, time_in_ns, latency_in_ns = (sim_dot_out_file)
                time_mean += int(time_in_ns)
                latency_mean += float(latency_in_ns)
                print("  time: {} | latency: {} | file: {}".format(time_in_ns, latency_in_ns, filename.split("simulations/",1)[1].replace(" ","")))
            time_mean = time_mean / meanings
            latency_mean = latency_mean / meanings
            print("  mean time: {}\n  mean ltcy: {}\n".format(time_mean, round(latency_mean,2)))
            line = [benchmark.name, "STTRAM", time_mean, latency_mean]
            self.dataframe.append(line)
    
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
    dataframe = pd.DataFrame(simulation.dataframe, columns=["benchmark","memory", "time", "latency"])
    
    palette ={"2D_RAM": "#0074D9", "3D_RAM": "#FF4136", "PCM": "#FFDC00", "RRAM": "#2ECC40", "STTRAM": "#FF851B"}
    
    graph = sns.barplot(x="benchmark", y="latency", hue="memory", data=dataframe, palette=palette)
    
    graph.set_title("Latência média de execução", weight='bold')
    graph.set_ylabel('LATÊNCIA (ns)', weight='bold')
    graph.set(ylim=(0, 500))
    graph.set_xlabel('')
    
    # rotations
    # plt.xticks(rotation=45)
    for container in graph.containers:
        graph.bar_label(container, fmt='%.2f', padding=2, rotation=75, size=7.5)

    plt.legend(title='', loc='best')
    graph.grid(axis="y", color='black', linewidth=0.075)
    mng = plt.get_current_fig_manager()
    mng.resize(*mng.window.maxsize())
    plt.show()

def generate_time_graph(simulation):
    dataframe = pd.DataFrame(simulation.dataframe, columns=["benchmark","memory", "time", "latency"])
    
    palette ={"2D_RAM": "#0074D9", "3D_RAM": "#FF4136", "PCM": "#FFDC00", "RRAM": "#2ECC40", "STTRAM": "#FF851B"}
    
    graph = sns.barplot(x="benchmark", y="time", hue="memory", data=dataframe, palette=palette)
    
    graph.set_title("Tempo médio de execução", weight='bold')
    graph.set_ylabel('TEMPO (ns) / 2DRAM', weight='bold')
    graph.set(ylim=(0, 5.5))
    graph.set_xlabel('')
    
    # rotations
    # plt.xticks(rotation=45)
    for container in graph.containers:
        graph.bar_label(container, fmt='%.2f', padding=2, rotation=75, size=7.5)

    plt.legend(title='', loc='best')
    graph.grid(axis="y", color='black', linewidth=0.075)
    mng = plt.get_current_fig_manager()
    mng.resize(*mng.window.maxsize())
    plt.show()

if __name__ == '__main__':
    simulations = []
    if len(sys.argv) < 2:
        print("[NVMSIM][ERROR] Missing arg to folder to simulations")
        exit()
    simulations_path = sys.argv[1]
    simulations_paths = glob.glob('{}/*'.format(simulations_path))
    for simulation_path in simulations_paths:
        name = simulation_path.rsplit('/', 1)[-1] 
        simulation = Simulation(name)
        simulation.path = simulation_path
        simulation.get_benchmarks_paths()
        simulation.get_sim_dot_outs_and_results()
        simulations.append(simulation)

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
    
    united_simulation.make_dataframe()
    united_simulation.normalize_time_values()
    generate_latency_graph(united_simulation)
    generate_time_graph(united_simulation)
