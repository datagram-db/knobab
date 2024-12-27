import bisect
from dataclasses import dataclass
from KeyWrapper import KeyWrapper

@dataclass
class Group:
    start:  int
    end:    int     #Inteval included
    span:   int
    def inInterval(self, x):
        return (self.start <= x) and (x <= self.end)
    @staticmethod
    def fromPair(begin, end):
        return Group(begin, end, (end - begin)+1)

    def asRange(self, step=1):
        return range(self.start, self.end-step+2)

    def asRangeSkipFirst(self, step=1):
        return range(self.start+1, self.end-step+2)

    def __len__(self):
        return self.span

class SortedIntervalTrees:
    def __init__(self, ls):
        self.ls = KeyWrapper(sorted(ls, key=lambda x: x.start), key=lambda x: x.start)
        self.n = len(self.ls)

    def __len__(self):
        return self.n

    def __getitem__(self, i):
        return self.ls.getRaw(i)
    def __iter__(self):
        return iter(self.ls)

    def inIntervalTree(self, x):
        if len(self.ls) == 0:
            return None
        idx = bisect.bisect(self.ls, x)
        if idx == -1:
            return None
        if (idx >= self.n) and (self.n>0):
            idx = self.n-1
        grp = self.ls.getRaw(idx)
        if idx != 0:
            curr = self.ls.getRaw(idx-1)
            if curr.inInterval(x):
                return curr
        return grp if grp.inInterval(x) else None
