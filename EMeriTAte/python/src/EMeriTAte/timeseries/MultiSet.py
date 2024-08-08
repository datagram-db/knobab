from collections import defaultdict


class MultiSet:
    def __init__(self):
        self.d = dict()

    def add(self, key):
        if key in self.d:
            self.d[key] += 1
        else:
            self.d[key] = 1

    def remove(self, key):
        if key in self.d:
            self.d[key] -= 1
            if self.d[key] == 0:
                del self.d[key]

    def numOfKeys(self):
        return len(self.d)

    def overallCardinality(self):
        return sum(self.d.values())

    def __iter__(self):
        return iter(self.d)

    def __getitem__(self, item):
        return self.cardinalityOf(item)

    def __len__(self):
        return len(self.d)

    def __contains__(self, item):
        return self.cardinalityOf(item) == 0

    def items(self):
        return self.d.items()

    def cardinalityOf(self, key):
        if key not in self.d:
            return 0
        else:
            return self.d[key]


class PolyadicTrace:
    def __init__(self):
        self.d = defaultdict(MultiSet)


    def add(self, label, offset):
        self.d[label].add(offset)

    def __iter__(self):
        return iter(self.d)

    def __getitem__(self, item):
        return self.getEventsInPositionalTrace(item)

    def items(self):
        return self.d.items()

    def keys(self):
        return self.d.keys()

    def getEventsInPositionalTrace(self, item):
        if item not in self.d:
            return MultiSet()
        else:
            return self.d[item]
