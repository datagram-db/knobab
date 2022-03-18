explain analyse
SELECT taska, taskb,
(CAST(COUNT(*) AS FLOAT) / CAST((SELECT COUNT(*) FROM log1 WHERE task LIKE taskb) AS FLOAT)) AS Support
FROM log1 a, (SELECT a.task AS taska, b.task AS taskb FROM log1 a, log1 b WHERE a.task != b.task GROUP BY a.task , b.task) x
WHERE a.task = x.taskb AND EXISTS (SELECT * FROM log1 b WHERE b.task = x.taska AND b.trace_id = a.trace_id AND b.time < a.time)
GROUP BY x.taska , x.taskb
HAVING (CAST(COUNT(*) AS FLOAT) /CAST( (SELECT COUNT(*) FROM log1 WHERE task LIKE taskb) AS FLOAT)) > 0.7