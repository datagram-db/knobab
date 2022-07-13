explain analyze
SELECT taska, taskb, ARRAY_AGG(DISTINCT TTID) as trace_ids
FROM (
SELECT x.taska, x.taskb, TTID
FROM  actions x, log1 a JOIN (select distinct(trace_id) as TTID from log1) tid_table ON a.trace_id = TTID
WHERE (a.task <> x.taska) OR 
      (a.task = x.taska AND EXISTS (SELECT * FROM log1 b WHERE b.task = x.taskb AND b.trace_id = a.trace_id AND b.timex > a.timex)
AND NOT EXISTS(SELECT * FROM log1 b , log1 c WHERE c.trace_id = a.trace_id AND c.task = x.taska AND b.trace_id = a.trace_id AND b.task = x.taskb AND c.timex > a.timex AND c.timex < b.timex))
GROUP BY x.taska, x.taskb, TTID
HAVING (CAST(COUNT(*) AS FLOAT) /CAST( (SELECT COUNT(*) FROM log1 WHERE trace_id = TTID) AS FLOAT))=1) QR
GROUP BY taska, taskb;
