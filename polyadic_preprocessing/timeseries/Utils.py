def grouped(iterable, n):
    "s -> (s0,s1,s2,...sn-1), (sn,sn+1,sn+2,...s2n-1), (s2n,s2n+1,s2n+2,...s3n-1), ..."
    return zip(*[iter(iterable)]*n)

def multi_group(L):
    N = min(map(lambda x : len(x), L.values()))
    result = []
    for i in range(N):
        collect = set()
        for key, ls in L.items():
            for x in ls[i]:
                collect.add(key+"("+x+")")
        if (len(collect) > 0):
            result.append(list(collect))
    return result