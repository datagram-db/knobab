-- SELECT 'ChainResponse', TaskA, TaskB,
-- (CAST(COUNT(*) AS FLOAT) / CAST((SELECT COUNT(*) FROM Log WHERE Task LIKE TaskA) AS FLOAT)) AS Support,
-- ((CAST(COUNT(*) AS FLOAT) / CAST((SELECT COUNT(*) FROM Log WHERE Task LIKE TaskA) AS FLOAT)) * (CAST((SELECT COUNT(*) FROM (SELECT Instance FROM Log WHERE Task LIKE TaskA
-- GROUP BY Instance) t2) AS FLOAT) / CAST((SELECT COUNT(*) FROM (SELECT Instance FROM Log GROUP BY Instance) t) AS FLOAT))) AS Confidence
-- FROM Log a, (SELECT a.Task AS TaskA, b.Task AS TaskB FROM Log a, Log b WHERE a.Task != b.Task GROUP BY a.Task , b.Task) x
-- WHERE a.Task = x.TaskA AND EXISTS (SELECT * FROM Log b WHERE b.Task = x.TaskB AND b.Instance = a.Instance AND b.Time > a.Time)
-- AND NOT EXISTS(SELECT * FROM Log b, Log c WHERE c.Instance = a.Instance AND b.Instance = a.Instance AND b.Task = x.TaskB AND c.Time > a.Time AND c.Time < b.Time)
-- GROUP BY x.TaskA, x.TaskB
-- HAVING (CAST(COUNT(*) AS FLOAT) / CAST( (SELECT COUNT(*) FROM Log WHERE Task LIKE TaskA) AS FLOAT)) > 0.7
-- AND (CAST(COUNT(*) AS FLOAT) /CAST( (SELECT COUNT(*) FROM Log WHERE Task LIKE TaskA) AS FLOAT)) * (CAST((SELECT COUNT(*) FROM(SELECT Instance FROM Log WHERE Task LIKE TaskA
-- GROUP BY Instance) t2) AS FLOAT) / CAST((SELECT COUNT(*) FROM(SELECT Instance FROM Log GROUP BY Instance) t) AS FLOAT))) > 0.5

-- Without confidence
SELECT taska, TaskB,
(CAST(COUNT(*) AS FLOAT) / CAST((SELECT COUNT(*) FROM log1 WHERE task LIKE taska) AS FLOAT)) AS Support
FROM log1 a, (SELECT a.task AS taska, b.task AS taskb FROM log1 a, log1 b WHERE a.task != b.task GROUP BY a.task , b.task) x
WHERE a.task = x.taska AND EXISTS (SELECT * FROM log1 b WHERE b.task = x.taskb AND b.trace_id = a.trace_id AND b.time > a.time)
AND NOT EXISTS(SELECT * FROM log1 b, log1 c WHERE c.trace_id = a.trace_id AND b.trace_id = a.trace_id AND b.task = x.taskb AND c.time > a.time AND c.time < b.time)
GROUP BY x.taska, x.taskb
HAVING (CAST(COUNT(*) AS FLOAT) / CAST( (SELECT COUNT(*) FROM log1 WHERE task LIKE taska) AS FLOAT)) > 0.7

-- With confidence
SELECT taska, TaskB,
(CAST(COUNT(*) AS FLOAT) / CAST((SELECT COUNT(*) FROM log1 WHERE task LIKE taska) AS FLOAT)) AS Support,
((CAST(COUNT(*) AS FLOAT) / CAST((SELECT COUNT(*) FROM log1 WHERE task LIKE taska) AS FLOAT)) * (CAST((SELECT COUNT(*) FROM (SELECT trace_id FROM log1 WHERE task LIKE taska
GROUP BY trace_id) t2) AS FLOAT) / CAST((SELECT COUNT(*) FROM (SELECT trace_id FROM log1 GROUP BY trace_id) t) AS FLOAT))) AS Confidence
FROM log1 a, (SELECT a.task AS taska, b.task AS taskb FROM log1 a, log1 b WHERE a.task != b.task GROUP BY a.task , b.task) x
WHERE a.task = x.taska AND EXISTS (SELECT * FROM log1 b WHERE b.task = x.taskb AND b.trace_id = a.trace_id AND b.time > a.time)
AND NOT EXISTS(SELECT * FROM log1 b, log1 c WHERE c.trace_id = a.trace_id AND b.trace_id = a.trace_id AND b.task = x.taskb AND c.time > a.time AND c.time < b.time)
GROUP BY x.taska, x.taskb
HAVING (CAST(COUNT(*) AS FLOAT) / CAST( (SELECT COUNT(*) FROM log1 WHERE task LIKE taska) AS FLOAT)) > 0.7
AND ((CAST(COUNT(*) AS FLOAT) /CAST( (SELECT COUNT(*) FROM log1 WHERE task LIKE taska) AS FLOAT)) * (CAST((SELECT COUNT(*) FROM(SELECT trace_id FROM log1 WHERE task LIKE taska
GROUP BY trace_id) t2) AS FLOAT) / CAST((SELECT COUNT(*) FROM(SELECT trace_id FROM log1 GROUP BY trace_id) t) AS FLOAT))) > 0.5

