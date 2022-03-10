-- SELECT 'Precedence', TaskA, TaskB,
-- (CAST(COUNT(*) AS FLOAT) / CAST((SELECT COUNT(*) FROM Log WHERE Task LIKE TaskB) AS FLOAT)) AS Support,
-- ((CAST(COUNT(*) AS FLOAT) / CAST((SELECT COUNT(*) FROM Log WHERE Task LIKE TaskB) AS FLOAT)) * (CAST((SELECT COUNT(*) FROM (SELECT Instance FROM Log WHERE Task LIKE TaskB
-- GROUP BY Instance) t2) AS FLOAT) / CAST( (SELECT COUNT(*) FROM (SELECT Instance FROM Log GROUP BY Instance) t) AS FLOAT))) AS Confidence
-- FROM Log a, (SELECT a.Task AS TaskA, b.Task AS TaskB FROM Log a, Log b WHERE a.Task != b.Task GROUP BY a.Task , b.Task) x
-- WHERE a.Task = x.TaskB AND EXISTS (SELECT * FROM Log b WHERE b.Task = x.TaskA AND b.Instance = a.Instance AND b.Time < a.Time)
-- GROUP BY x.TaskA , x.TaskB
-- HAVING (CAST(COUNT(*) AS FLOAT) /CAST( (SELECT COUNT(*) FROM Log WHERE Task LIKE TaskB) AS FLOAT)) > 0.7
-- AND ((CAST(COUNT(*) AS FLOAT) / CAST( (SELECT COUNT(*) FROM Log WHERE Task LIKE TaskB) AS FLOAT)) * (CAST((SELECT COUNT(*) FROM(SELECT Instance FROM Log WHERE Task LIKE TaskB
-- GROUP BY Instance) t2) AS FLOAT) / CAST((SELECT COUNT(*) FROM(SELECT Instance FROM Log GROUP BY Instance) t) AS FLOAT))) > 0.5

-- Without confidence
SELECT taska, taskb,
(CAST(COUNT(*) AS FLOAT) / CAST((SELECT COUNT(*) FROM log1 WHERE task LIKE taskb) AS FLOAT)) AS Support
FROM log1 a, (SELECT a.task AS taska, b.task AS taskb FROM log1 a, log1 b WHERE a.task != b.task GROUP BY a.task , b.task) x
WHERE a.task = x.taskb AND EXISTS (SELECT * FROM log1 b WHERE b.task = x.taska AND b.trace_id = a.trace_id AND b.time < a.time)
GROUP BY x.taska , x.taskb
HAVING (CAST(COUNT(*) AS FLOAT) /CAST( (SELECT COUNT(*) FROM log1 WHERE task LIKE taskb) AS FLOAT)) > 0.7

-- With confidence
SELECT taska, taskb,
(CAST(COUNT(*) AS FLOAT) / CAST((SELECT COUNT(*) FROM log1 WHERE task LIKE taskb) AS FLOAT)) AS Support,
((CAST(COUNT(*) AS FLOAT) / CAST((SELECT COUNT(*) FROM log1 WHERE task LIKE taskb) AS FLOAT)) * (CAST((SELECT COUNT(*) FROM (SELECT trace_id FROM log1 WHERE task LIKE taskb
GROUP BY trace_id) t2) AS FLOAT) / CAST( (SELECT COUNT(*) FROM (SELECT trace_id FROM log1 GROUP BY trace_id) t) AS FLOAT))) AS Confidence
FROM log1 a, (SELECT a.task AS taska, b.task AS taskb FROM log1 a, log1 b WHERE a.task != b.task GROUP BY a.task , b.task) x
WHERE a.task = x.taskb AND EXISTS (SELECT * FROM log1 b WHERE b.task = x.taska AND b.trace_id = a.trace_id AND b.time < a.time)
GROUP BY x.taska , x.taskb
HAVING (CAST(COUNT(*) AS FLOAT) /CAST( (SELECT COUNT(*) FROM log1 WHERE task LIKE taskb) AS FLOAT)) > 0.7
AND ((CAST(COUNT(*) AS FLOAT) / CAST( (SELECT COUNT(*) FROM log1 WHERE task LIKE taskb) AS FLOAT)) * (CAST((SELECT COUNT(*) FROM(SELECT trace_id FROM log1 WHERE task LIKE taskb
GROUP BY trace_id) t2) AS FLOAT) / CAST((SELECT COUNT(*) FROM(SELECT trace_id FROM log1 GROUP BY trace_id) t) AS FLOAT))) > 0.5