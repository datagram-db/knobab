from prefixspan import PrefixSpan
from pycspade.helpers import spade, print_result

from timeseries.DatalessDeclareEmbedding import DeclareDevMining
from timeseries.Log import Log
from timeseries.Utils import multi_group


class MiningConfiguration:
    def __init__(self, algorithm, maxlen=None):
        self.algorithm =algorithm
        self.maxlen = maxlen

    @staticmethod
    def spade(support=0.01, maxsize=None, maxlen=None, mingap=None):
        conf = MiningConfiguration("spade", maxlen=maxlen)
        conf.support = support
        conf.maxsize = maxsize
        conf.mingap = mingap
        return conf

    @staticmethod
    def polyadic_spade(support=0.01, maxsize=None, maxlen=None, mingap=None):
        conf = MiningConfiguration("polyadic_spade", maxlen=maxlen)
        conf.support = support
        conf.maxsize = maxsize
        conf.mingap = mingap
        return conf

    @staticmethod
    def prefixMiningFrequent(support, maxlen=None):
        conf = MiningConfiguration("prefixspan", maxlen=maxlen)
        conf.type = "frequent"
        conf.support = support
        return conf

    @staticmethod
    def declareDatalessMining(support, maxlen=None):
        conf = MiningConfiguration("declare_dataless", maxlen=None)
        conf.type = "frequent"
        conf.support = support
        return conf

    @staticmethod
    def declareDatalessPolyadicMining(support, maxlen=None):
        conf = MiningConfiguration("polyadic_declare_dataless", maxlen=None)
        conf.type = "frequent"
        conf.support = support
        return conf

    @staticmethod
    def prefixMiningTopK(k, maxlen=None):
        conf = MiningConfiguration("prefixspan", maxlen=maxlen)
        conf.type = "topk"
        conf.k = k
        return conf

    @classmethod
    def episodical(cls, param):
        conf = MiningConfiguration("episodical", maxlen=None)
        return conf

def sublist(L, s):
    tmp = sum(L[i:i+len(s)]==s for i in range(len(L)))
    if tmp==0:
        tmp = -1
    return tmp

def combineResults(all_results, filed_name, s, Ldb, Rdb):
    left_label = 1
    right_label = 0
    if True:
    # assert left_label != right_label
    # for field_name, patterns in L.outcome_mining.items():
        for idx, trace in enumerate(Ldb):
            d = dict()
            all_results[idx]["class"] = left_label
            for pattern in s:
                score, frequent_item = pattern
                k = str(filed_name) + "(" + str(frequent_item) + ")"
                all_results[idx][k] = sublist(frequent_item, trace)
        N = len(Ldb)
        for idx, trace in enumerate(Rdb):
            all_results[idx]["class"] = right_label
            for pattern in s:
                score, frequent_item = pattern
                k = str(filed_name) + "(" + str(frequent_item) + ")"
                all_results[N+idx][k] = sublist(frequent_item, trace)
    return  combineResults

class SequentialPatternMining:
    def __init__(self, db):
        self.db = db
        self.prefixSpan = None

    @staticmethod
    def collect_candidates(r, omL, omR, conf):
        h = set()
        for x in omL.outcome_mining.keys():
            h.add(x)
        for x in omR.outcome_mining.keys():
            h.add(x)
        for x in h:
            if x not in r:
                r[x] = SequentialPatternMining.jointly_candidates(x, omL.outcome_mining[x], omR.outcome_mining[x], conf)
            else:
                r[x] = r[x].union(SequentialPatternMining.jointly_candidates(x, omL.outcome_mining[x], omR.outcome_mining[x], conf))
        return r

    @staticmethod
    def jointly_candidates(label, pos, neg, conf):
        if conf.algorithm == "prefixspan":
            supp = conf.support
            if conf.type == "frequent":
                conf.support = round(len(pos.db) * supp)
            else:
                conf.support = supp
            PR = pos.prefixSpanMining(conf)
            if conf.type == "frequent":
                conf.support = round(len(neg.db) * supp)
            else:
                conf.support = supp
            NR = neg.prefixSpanMining(conf)
            s = set()
            for x in PR:
                s.add(tuple(x[1]))
            for x in NR:
                s.add(tuple(x[1]))
            return s
            # combineResults(all_results, label, s, pos.db, neg.db)
        elif conf.algorithm == "spade":
            PR = pos.spade(conf)
            NR = neg.spade(conf)
            s = set()
            for x in PR:
                s.add(tuple(x[1]))
            for x in NR:
                s.add(tuple(x[1]))
            return s
            # combineResults(all_results, label, s, pos.db, neg.db)
        elif conf.algorithm == "declare_dataless":
            lPos = Log(withData=False, log=pos.db, isTab=True, isList=True)
            lNeg = Log(withData=False, log=neg.db, isTab=True, isList=True)
            miner = DeclareDevMining()
            return miner.collect_candidates(lPos, lNeg, conf, candidate_threshold=conf.support)
        elif conf.algorithm == "episodical":
            PR = pos.episodical(conf)
            NR = neg.episodical(conf)
            s = set()
            for x in PR:
                s.add(tuple(x[1]))
            for x in NR:
                s.add(tuple(x[1]))
            return s
        return set()
            # combineResults(all_results, label, s, pos.db, neg.db)


    # def mining(self, conf:MiningConfiguration):
    #     if conf.algorithm == "prefixspan":
    #         if conf.type == "frequent":
    #             conf.support = round(len(self.db) * conf.support)
    #         return self.prefixSpanMining(conf)
    #     elif conf.algorithm == "spade":
    #         return self.spade(conf)
    #     elif conf.algorithm == "declare_dataless":
    #         return self.declare_dataless(conf)

    def prefixSpanMining(self, conf:MiningConfiguration):
        self.prefixSpan = PrefixSpan(self.db)
        N = len(self.db)
        self.prefixSpan.maxlen = conf.maxlen
        localResult = []
        if conf.type == "frequent":
            localResult = self.prefixSpan.frequent(conf.support)
        elif conf.type == "topk":
            localResult = self.prefixSpan.topk(conf.k)
        return list(map(lambda x : tuple([x[0]/N, x[1]]), sorted(localResult, key=lambda x: x[0], reverse=True)))

    def declare_dataless(self, conf):
        assert False

    def episodical(self, conf):
        string_to_count = dict()
        for start_time, d in self.db.items():
            for length, v in d.items():
                for x in v:
                    if x.name in string_to_count:
                        string_to_count[x.name].add(x.start_time)
                    else:
                        string_to_count[x.name] = set()
                        string_to_count[x.name].add(x.start_time)
        result = [tuple([len(string_to_count[x])/conf.maxLen, [x]]) for x in string_to_count]
        return sorted(result, key=lambda x: x[0], reverse=True)

    def spade_polyadic(self, conf:MiningConfiguration):
        stringToNumber = dict()
        number_to_string = []
        data = []
        count = 1
        for i, ls in enumerate(self.db):
            for j, event in enumerate(ls):
                for item in event:
                    if item not in stringToNumber:
                        stringToNumber[item] = count
                        number_to_string.append(item)
                        count = count+1
        for i, ls in enumerate(self.db):
            for j, event in enumerate(ls):
                data.append([i, j, list(map(lambda x: stringToNumber[x], event))])
        internal_data = spade(data=data, support=conf.support, maxlen=conf.maxlen, mingap=conf.mingap, maxsize=conf.maxsize)
        result = []
        # print_result(internal_data)
        nseqs = internal_data['nsequences']
        for mined_object in internal_data["mined_objects"]:
            L = []
            for item in mined_object.items:
                W = []
                for element in item.elements:
                    W.append(number_to_string[element-1])
                L.append(tuple(W))
            result.append(tuple([mined_object.noccurs/nseqs, L]))
        return result

    def spade(self, conf:MiningConfiguration):
        stringToNumber = dict()
        number_to_string = []
        data = []
        count = 1
        for i, list in enumerate(self.db):
            for j, item in enumerate(list):
                if item not in stringToNumber:
                    stringToNumber[item] = count
                    number_to_string.append(item)
                    data.append([i, j, [count]])
                    count = count+1
                else:
                    data.append([i,j, [stringToNumber[item]]])
        internal_data = spade(data=data, support=conf.support, maxlen=conf.maxlen, mingap=conf.mingap, maxsize=conf.maxsize)
        result = []
        nseqs = internal_data['nsequences']
        for mined_object in internal_data["mined_objects"]:
            L = []
            for item in mined_object.items:
                for element in item.elements:
                    L.append(number_to_string[element-1])
            result.append(tuple([mined_object.noccurs/nseqs, L]))
        return result

    @staticmethod
    def probe_candidates_polyadic(candidates, L, R, conf):
        if conf.algorithm.endswith("spade"):
            L = SequentialPatternMining(L)
            PR = L.spade_polyadic(conf)
            R = SequentialPatternMining(R)
            NR = R.spade_polyadic(conf)
            ## TODO: check for the embedding which event contain the patterns given by the matcher
        elif conf.algorithm.endswith("declare_dataless"):
            lPos = Log(withData=False, log=L, isTab=True, isList=True)
            lNeg = Log(withData=False, log=R, isTab=True, isList=True)
            miner = DeclareDevMining()
            miner.run2(None, lPos, lNeg)

    @staticmethod
    def probe_candidates(candidates, dict_list, L, R, conf):
        dpos = dict()
        dpos["class"] = 1
        dneg = dict()
        dneg["class"] = 0
        if conf.algorithm.startswith("polyadic_full"):
            dict_list.append(L.log.asPolyadicTrace(1))
            dict_list.append(R.log.asPolyadicTrace(1))
            return (True,True)
        elif conf.algorithm.startswith("polyadic_"):
            dict_list.append(multi_group(L.outcome_mining))
            dict_list.append(multi_group(R.outcome_mining))
            return (True,False)
        for type, patterns in candidates.items():
            ldb = None
            rdb = None
            if type in L.outcome_mining:
                ldb = L.outcome_mining[type]
            if type in R.outcome_mining:
                rdb = R.outcome_mining[type]
            if conf.algorithm == "prefixspan":
                pass
                # combineResults(all_results, label, s, ldb, rdb)
            elif conf.algorithm == "spade":
                pass
                # combineResults(all_results, label, s, ldb, rdb)
            elif conf.algorithm == "declare_dataless":
                lPos = None
                lNeg = None
                if ldb is not None:
                    lPos = Log(withData=False, log=ldb, isTab=True, isList=True)
                if rdb is not None:
                    lNeg = Log(withData=False, log=rdb, isTab=True, isList=True)
                miner = DeclareDevMining()
                # return miner.collect_candidates(label, lPos, lNeg, all_results, candidate_threshold=conf.support)
            elif conf.algorithm == "episodical":
                for pattern in patterns:
                    k = type + "(" + pattern[0] + ")"
                    if (ldb is not None) and (ldb.db is not None):
                        for time, len_patterns in ldb.db.items():
                            for len, mined in len_patterns.items():
                                for x in mined:
                                    if type + "(" +x.name+ ")" == k:
                                        if k in dpos:
                                            dpos[k] += 1
                                        else:
                                            dpos[k] = 1
                    if (rdb is not None) and (rdb.db is not None):
                        for time, len_patterns in rdb.db.items():
                            for len, mined in len_patterns.items():
                                for x in mined:
                                    if type + "(" +x.name+ ")" == k:
                                        if k in dneg:
                                            dneg[k] += 1
                                        else:
                                            dneg[k] = 1
        dict_list.append(dpos)
        dict_list.append(dneg)
        return (False,False)
                # combineResults(all_results, label, s, pos.db, neg.db)


