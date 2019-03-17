use [Task4]

--Получить список всех должностей компании с количеством сотрудников на каждой из них
select p.position_name as [Position name],  count(t.id_Position) as [Count]
from Position p
left join Task t on p.id=t.id_Position
group by p.position_name

--Определить список должностей компании, на которых нет сотрудников 
select p.position_name as [Position name],  count(t.id_Position) as [Count]
from Position p
left join Task t on p.id=t.id_Position
group by p.position_name
having count(t.id_Position)=0

--Получить список проектов с указанием, сколько сотрудников каждой должности работает на проекте
select p.project_name as [Project name], count(t.id_worker) as [Count]
from Project p
left join Task t on p.id=t.idProject
group by p.project_name

--Посчитать на каждом проекте, какое в среднем количество задач приходится на каждого сотрудника
select p.id, p.project_name as [Project name], t.id_worker, count(t.id_worker) as [NumberOfTasksOnTheProject]
from Project p
left join Task t on p.id=t.idProject
group by p.id, p.project_name, t.id_worker
order by 1,3

select t.id_worker as [Id], avg(t.[NumberOfTasksOnTheProject]) as [AverageNumberTasksOnTheProject]
from (
		select p.id, p.project_name, t.id_worker, count(t.id_worker) as [NumberOfTasksOnTheProject]
		from Project p
		left join Task t on p.id=t.idProject
		group by p.id, p.project_name, t.id_worker
	) t
group by t.id_worker
order by 1

--Подсчитать длительность выполнения каждого проекта
select p.project_name as [Project name], case when p.p_closedate is null then 'Closing date  is not available.' else convert(nvarchar(20),DATEDIFF(day,p.p_createdate, p.p_closedate ))
end as [Project Duration]
from Project p

--Определить сотрудников с минимальным количеством незакрытых задач
select w.worker_lastName as [Last name], count(t.idTaskstatuslog) as [Count]
from Workers w
left join Task t on w.id=t.id_worker
inner join TaskStatusLog tsl on t.idTaskstatuslog=tsl.id and tsl.idTaskstatus<>4
group by w.worker_lastName
order by count(t.idTaskstatuslog)

--Определить сотрудников с максимальным количеством незакрытых задач, дедлайн которых уже истек
select w.worker_lastName as [Last name], count(t.idTaskstatuslog) as [Count]
from Workers w
left join Task t on w.id=t.id_worker
inner join TaskStatusLog tsl on t.idTaskstatuslog=tsl.id and tsl.idTaskstatus<>4
where t.t_deathline_date>getdate()
group by w.worker_lastName
order by count(t.idTaskstatuslog) desc

--Продлить дедлайн незакрытых задач на 5 дней
update t
set t_deathline_date = DATEADD(day,5, t.t_deathline_date)
from Task t
inner join TaskStatusLog tsl on t.idTaskstatuslog=tsl.id and tsl.idTaskstatus<>4

--Перевести проекты в состояние закрыт, для которых все задачи закрыты и задать время закрытия временем закрытия задачи проекта, принятой последней
update p
set idProjectStatus=2, p_closedate=LastDateClosing.data
from Project p
outer apply (select count(t.idProject) number
             from Task t 
			 where p.id=t.idProject
             ) NumberOfTasks
outer apply (select count(t.idProject) number, max(tsl.setTaskDate) LastDateClosing
             from Task t 
             inner join TaskStatusLog tsl on t.idTaskstatuslog=tsl.id and tsl.idTaskstatus=4
			 where p.id=t.idProject
             ) NumberOfClosedTasks
outer apply (select max(tsl.setTaskDate) data
             from Task t 
             inner join TaskStatusLog tsl on t.idTaskstatuslog=tsl.id and tsl.idTaskstatus=4
			 where p.id=t.idProject
             ) LastDateClosing
where NumberOfTasks.number = NumberOfClosedTasks.number

--Выяснить по всем проектам, какие сотрудники на проекте не имеют незакрытых задач
select w.* 
from Project p
inner join Task t on p.id=t.idProject
inner join TaskStatusLog tsl on t.idTaskstatuslog=tsl.id and tsl.idTaskstatus=4
inner join Workers w on t.id_worker=w.id

--Заданную задачу (по названию) проекта перевести на сотрудника с минимальным количеством выполняемых им задач 

declare @pn int = 2 
declare @nz int = 6
update t
set id_worker = (select top 1 t.id_worker from Task t where t.idProject=@pn group by t.idProject, t.id_worker order by count(t.id_worker))
from Task t
where t.idProject=@pn and t.id=@nz