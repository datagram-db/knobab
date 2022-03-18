explain analyse
SELECT taska, taskb,
(CAST(COUNT(*) AS FLOAT) / CAST((SELECT COUNT(*) FROM log1 WHERE task LIKE taska) AS FLOAT)) AS Support
FROM log1 a, (SELECT a.task AS taska, b.task AS taskb FROM log1 a, log1 b WHERE a.task != b.task GROUP BY a.task, b.task) x
WHERE a.task = x.taskb AND a.time < ALL (SELECT time FROM log1 b WHERE b.task = x.taska AND b.trace_id = a.trace_id)
AND EXISTS (SELECT * FROM log1 b WHERE b.task = x.taska AND b.trace_id = a.trace_id) AND a.time > ALL(SELECT time FROM log1 b WHERE b.task = x.taskb AND b.trace_id = a.trace_id)
GROUP BY x.taska, x.taskb
HAVING (CAST(COUNT(*) AS FLOAT) / CAST((SELECT COUNT(*) FROM log1 WHERE task LIKE taska) AS FLOAT)) > 0.7