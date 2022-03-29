explain analyse
SELECT taska, TaskB,
(CAST(COUNT(*) AS FLOAT) / CAST((SELECT COUNT(*) FROM log1 WHERE task LIKE taska) AS FLOAT)) AS Support
FROM log1 a, (SELECT a.task AS taska, b.task AS taskb FROM log1 a, log1 b WHERE a.task != b.task GROUP BY a.task , b.task) x
WHERE a.task = x.taska AND EXISTS (SELECT * FROM log1 b WHERE b.task = x.taskb AND b.trace_id = a.trace_id AND b.time > a.time)
AND NOT EXISTS(SELECT * FROM log1 b, log1 c WHERE c.trace_id = a.trace_id AND b.trace_id = a.trace_id AND b.task = x.taskb AND c.time > a.time AND c.time < b.time)
GROUP BY x.taska, x.taskb
HAVING (CAST(COUNT(*) AS FLOAT) / CAST( (SELECT COUNT(*) FROM log1 WHERE task LIKE taska) AS FLOAT)) > 0.7

