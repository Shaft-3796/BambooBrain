import math

import matplotlib.pyplot as plt
import numpy as np

PATH = "datasets/FASHION_test.txt"


class Instance:

    def __init__(self, n_class: int, features: list):
        self.n_class = n_class
        self.features = features

    @classmethod
    def from_line(cls, line: str):
        line = line.split(" ")
        first = line[0].split('\t')
        n_class = int(first[0])
        line[0] = first[1]
        features = [int(x) for x in line[0:-1]]
        return cls(n_class, features)

    @property
    def img(self):
        return np.array(self.features).reshape((int(math.sqrt(len(self.features))), int(math.sqrt(len(self.features)))))

    def plot(self):
        plt.imshow(self.img, cmap='gray')
        plt.show()


class Dataset:

    def __init__(self, instances_size: int, classes_size: int, features_size: int):
        self.instances_size = instances_size
        self.classes_size = classes_size
        self.features_size = features_size
        self.instances = []

    def add_instance(self, instance: Instance):
        self.instances.append(instance)

    def add_instances(self, instances: list):
        self.instances += instances

    @classmethod
    def from_file(cls, path):
        with open(path, "r") as f:
            lines = f.readlines()
            header = lines[0].split(" ")
            dataset = Dataset(int(header[0]), int(header[1]), int(header[2]))
            for line in lines[1:]:
                dataset.add_instance(Instance.from_line(line))
        return dataset

    def plot(self, start: int, end: int):
        size = int(math.sqrt(end - start))
        fig, axes = plt.subplots(size, size)

        for axs in axes:
            for ax in axs:
                ax.imshow(self.instances[start].img, cmap="gray")
                start += 1
                if start == end:
                    break

        plt.show()


ds = Dataset.from_file(PATH)
ds.plot(0, 32)