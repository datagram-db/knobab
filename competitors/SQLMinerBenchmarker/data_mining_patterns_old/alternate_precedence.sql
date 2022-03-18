-- TODO Not working for AB and BA, log_until.txt

--Without confidence
explain analyse
SELECT taska, taskb,
(CAST(COUNT(*) AS FLOAT) / CAST((SELECT COUNT(*) FROM log1 WHERE task LIKE taskb) AS FLOAT)) AS Support
FROM log1 a, (SELECT a.task AS taska , b.task AS taskb FROM log1 a, log1 b WHERE a.task != b.task GROUP BY a.task, b.task) x
WHERE a.task = x.taskb AND EXISTS (SELECT * FROM log1 b WHERE b.task = x.taska AND b.trace_id = a.trace_id AND b.time < a.time)
AND NOT EXISTS(SELECT * FROM log1 b, log1 c WHERE c.trace_id = a.trace_id AND c.task = x.taskb AND b.trace_id = a.trace_id AND b.task = x.taska AND c.time < a.time AND c.time > b.time)
GROUP BY x.taska , x.taskb
HAVING (CAST(COUNT(*) AS FLOAT) / CAST((SELECT COUNT(*) FROM log1 WHERE task LIKE taskb) AS FLOAT)) > 0.7

-- SELECT 'alternatePrecedence', TaskA, TaskB,
-- (CAST(COUNT(*) AS FLOAT) / CAST((SELECT COUNT(*) FROM Log WHERE Task LIKE TaskB) AS FLOAT)) AS Support,
-- ((CAST(COUNT(*) AS FLOAT) / CAST((SELECT COUNT(*) FROM Log WHERE Task LIKE TaskB) AS FLOAT)) * (CAST((SELECT COUNT(*) FROM (SELECT Instance FROM Log WHERE Task LIKE TaskB
-- GROUP BY Instance) t2) AS FLOAT) / CAST((SELECT COUNT(*) FROM (SELECT Instance FROM Log GROUP BY Instance) t) AS FLOAT))) AS Confidence
-- FROM Log a, (SELECT a.Task AS TaskA , b.Task AS TaskB FROM Log a, Log b WHERE a.Task != b.Task GROUP BY a.Task, b.Task) x
-- WHERE a.Task = x.TaskB AND EXISTS (SELECT * FROM Log b WHERE b.Task = x.TaskA AND b.Instance = a.Instance AND b.Time < a.Time)
-- AND NOT EXISTS(SELECT * FROM Log b, Log c WHERE c.Instance = a.Instance AND c.Task = x.TaskB AND b.Instance = a.Instance AND b.Task = x.TaskA AND c.Time < a.Time AND c.Time > b.Time)
-- GROUP BY x.TaskA , x.TaskB
-- HAVING (CAST(COUNT(*) AS FLOAT) / CAST((SELECT COUNT(*) FROM Log WHERE Task LIKE TaskB) AS FLOAT)) > 0.7 AND
-- ((CAST(COUNT(*) AS FLOAT) / CAST((SELECT COUNT(*) FROM Log WHERE Task LIKE TaskB) AS FLOAT)) * (CAST((SELECT COUNT(*) FROM(SELECT Instance FROM Log WHERE Task LIKE TaskB
-- GROUP BY Instance) t2) AS FLOAT) / CAST((SELECT COUNT(*) FROM(SELECT Instance FROM Log GROUP BY Instance) t) AS FLOAT))) > 0.5

-- AB = 01111111111111111101 = 18 / 20 = 0.900 ?
-- AC = 0111 = 3 / 4 = 0.75
-- BA = 000000001000100010001 = 4 / 21 = 0.190 ?
-- CA = 000000001000000000000 = 1 / 21 = 0.0476
-- CB = 00000100000000000000 = 1 / 20 = 0.05

--With confidence
-- SELECT taska, taskb,
-- (CAST(COUNT(*) AS FLOAT) / CAST((SELECT COUNT(*) FROM log1 WHERE task LIKE taskb) AS FLOAT)) AS Support,
-- ((CAST(COUNT(*) AS FLOAT) / CAST((SELECT COUNT(*) FROM log1 WHERE task LIKE taskb) AS FLOAT)) * (CAST((SELECT COUNT(*) FROM (SELECT trace_id FROM log1 WHERE task LIKE taskb
-- GROUP BY trace_id) t2) AS FLOAT) / CAST((SELECT COUNT(*) FROM (SELECT trace_id FROM log1 GROUP BY trace_id) t) AS FLOAT))) AS Confidence
-- FROM log1 a, (SELECT a.task AS taska , b.task AS taskb FROM log1 a, log1 b WHERE a.task != b.task GROUP BY a.task, b.task) x
-- WHERE a.task = x.taskb AND EXISTS (SELECT * FROM log1 b WHERE b.task = x.taska AND b.trace_id = a.trace_id AND b.time < a.time)
-- AND NOT EXISTS(SELECT * FROM log1 b, log1 c WHERE c.trace_id = a.trace_id AND c.task = x.taskb AND b.trace_id = a.trace_id AND b.task = x.taska AND c.time < a.time AND c.time > b.time)
-- GROUP BY x.taska , x.taskb
-- HAVING (CAST(COUNT(*) AS FLOAT) / CAST((SELECT COUNT(*) FROM log1 WHERE task LIKE taskb) AS FLOAT)) > 0.7 AND
-- ((CAST(COUNT(*) AS FLOAT) / CAST((SELECT COUNT(*) FROM log1 WHERE task LIKE taskb) AS FLOAT)) * (CAST((SELECT COUNT(*) FROM(SELECT trace_id FROM log1 WHERE task LIKE taskb
-- GROUP BY trace_id) t2) AS FLOAT) / CAST((SELECT COUNT(*) FROM(SELECT trace_id FROM log1 GROUP BY trace_id) t) AS FLOAT))) > 0.5