-- TODO Not working for AB and BA, log_until.txt

-- Without confidence
explain analyse
SELECT taska, taskb,
(CAST(COUNT(*) AS FLOAT) / CAST((SELECT COUNT(*) FROM log1 WHERE Task LIKE taska) AS FLOAT)) AS Support
FROM log1 a, (SELECT a.task AS taska, b.task AS taskb FROM log1 a, log1 b WHERE a.task != b.task GROUP BY a.task, b.task) x
WHERE a.task = x.taskb AND EXISTS (SELECT * FROM log1 b WHERE b.task = x.taska AND b.trace_id = a.trace_id)
GROUP BY x.taska, x.taskb
HAVING (CAST(COUNT(*) AS FLOAT) / CAST((SELECT COUNT(*) FROM log1 WHERE Task LIKE taska) AS FLOAT)) > 0.7

-- With confidence
-- SELECT taska, taskb,
-- (CAST(COUNT(*) AS FLOAT) / CAST((SELECT COUNT(*) FROM log1 WHERE Task LIKE taska) AS FLOAT)) AS Support,
-- ((CAST(COUNT(*) AS FLOAT) / CAST((SELECT COUNT(*) FROM log1 WHERE Task LIKE taska) AS FLOAT)) * (CAST((SELECT COUNT(*) FROM (SELECT trace_id FROM log1 WHERE Task LIKE taska
-- GROUP BY trace_id) t2) AS FLOAT) / CAST((SELECT COUNT(*) FROM (SELECT trace_id FROM log1 GROUP BY trace_id) t) AS FLOAT))) AS Confidence
-- FROM log1 a, (SELECT a.task AS taska , b.task AS taskb FROM log1 a, log1 b WHERE a.task != b.task GROUP BY a.task, b.task) x
-- WHERE a.task = x.taskb AND EXISTS (SELECT * FROM log1 b WHERE b.task = x.taska AND b.trace_id = a.trace_id)
-- GROUP BY x.taska, x.taskb
-- HAVING (CAST(COUNT(*) AS FLOAT) / CAST((SELECT COUNT(*) FROM log1 WHERE Task LIKE taska) AS FLOAT)) > 0.7
-- AND ((CAST(COUNT(*) AS FLOAT) / CAST((SELECT COUNT(*) FROM log1 WHERE Task LIKE taska) AS FLOAT)) * (CAST((SELECT COUNT (*) FROM(SELECT trace_id FROM log1 WHERE Task LIKE taska
-- GROUP BY trace_id) t2) AS FLOAT) / CAST((SELECT COUNT(*) FROM(SELECT trace_id FROM log1 GROUP BY trace_id) t) AS FLOAT))) > 0.5

-- SELECT 'respondedExistence', TaskA, TaskB,
-- CAST(COUNT(*) AS FLOAT) / CAST((SELECT COUNT(*) FROM Log WHERE Task LIKE TaskA) AS FLOAT)) AS Support,
-- ((CAST(COUNT(*) AS FLOAT) / CAST((SELECT COUNT(*) FROM Log WHERE Task LIKE TaskA) AS FLOAT)) * (CAST((SELECT COUNT(*) FROM (SELECT Instance FROM Log WHERE Task LIKE TaskA
-- GROUP BY Instance) t2) AS FLOAT) / CAST((SELECT COUNT(*) FROM (SELECT Instance FROM Log GROUP BY Instance) t) AS FLOAT))) AS Confidence
-- FROM Log a, (SELECT a.Task AS TaskA , b.Task AS TaskB FROM Log a, Log b WHERE a.Task != b.Task GROUP BY a.Task, b.Task) x
-- WHERE a.Task = x.TaskB AND EXISTS (SELECT * FROM Log b WHERE b.Task = x.TaskA AND b.Instance = a.Instance)
-- GROUP BY x.TaskA, x.TaskB
-- HAVING (CAST(COUNT(*) AS FLOAT) / CAST((SELECT COUNT(*) FROM Log WHERE Task LIKE TaskA) AS FLOAT)) > 0.7
-- AND ((CAST(COUNT(*) AS FLOAT) / CAST((SELECT COUNT(*) FROM Log WHERE Task LIKE TaskA) AS FLOAT)) * (CAST((SELECT COUNT (*) FROM(SELECT Instance FROM Log WHERE Task LIKE TaskA
-- GROUP BY Instance) t2) AS FLOAT) / CAST((SELECT COUNT(*) FROM(SELECT Instance FROM Log GROUP BY Instance) t) AS FLOAT))) > 0.5

-- AB = 010101111111111111111 = 18 / 21 = 0.857 ?
-- AC = 001010100000000000000 = 3 / 21 = 0.142
-- BA = 01111111111111111111 = 19 / 20 = 0.95 ?
-- BC = 00000100000000000000 = 1 / 20 = 0.05
-- CA = 0111 = 3 / 4 = 0.75 ?
-- CB = 0001 = 1 / 4 = 0.25