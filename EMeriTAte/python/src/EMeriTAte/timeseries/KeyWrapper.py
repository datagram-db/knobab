class KeyWrapper:
    """
    @author: https://stackoverflow.com/a/39501468/1376095
    """
    def __init__(self, iterable, key):
        self.it = iterable
        self.key = key

    def __iter__(self):
        return iter(self.it)

    def __getitem__(self, i):
        return self.key(self.it[i])
    def getRaw(self, i):
        return self.it[i]
    def __len__(self):
        return len(self.it)
    def insert(self, index, item):
        print('asked to insert %s at index%d' % (item, index))
        self.it.insert(index, {"time":item})

