use [Task4]

-- Get a list of all company posts with the number of employees in each of them
select p.position_name as [Position name],  count(t.id_Position) as [Count]
from Position p
left join Task t on p.id=t.id_Position
group by p.position_name

-- Identify a list of company posts with no employees
select p.position_name as [Position name],  count(t.id_Position) as [Count]
from Position p
left join Task t on p.id=t.id_Position
group by p.position_name
having count(t.id_Position)=0

-- Get a list of projects with an indication of how many employees of each position work on the project
select p.project_name as [Project name], count(t.id_worker) as [Count]
from Project p
left join Task t on p.id=t.idProject
group by p.project_name

-- Count on each project, what is the average number of tasks per employee
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

-- Count the duration of each project
select p.project_name as [Project name], case when p.p_closedate is null then 'Closing date  is not available.' else convert(nvarchar(20),DATEDIFF(day,p.p_createdate, p.p_closedate ))
end as [Project Duration]
from Project p

-- Identify employees with a minimum number of unclosed tasks
select w.worker_lastName as [Last name], count(t.idTaskstatuslog) as [Count]
from Workers w
left join Task t on w.id=t.id_worker
inner join TaskStatusLog tsl on t.idTaskstatuslog=tsl.id and tsl.idTaskstatus<>4
group by w.worker_lastName
order by count(t.idTaskstatuslog)

-- Identify employees with the maximum number of unclosed tasks whose deadline has expired
select w.worker_lastName as [Last name], count(t.idTaskstatuslog) as [Count]
from Workers w
left join Task t on w.id=t.id_worker
inner join TaskStatusLog tsl on t.idTaskstatuslog=tsl.id and tsl.idTaskstatus<>4
where t.t_deathline_date>getdate()
group by w.worker_lastName
order by count(t.idTaskstatuslog) desc

-- Extend deadline for unclosed tasks for 5 days
update t
set t_deathline_date = DATEADD(day,5, t.t_deathline_date)
from Task t
inner join TaskStatusLog tsl on t.idTaskstatuslog=tsl.id and tsl.idTaskstatus<>4

-- Transfer projects to the closed state, for which all tasks are closed and set the closing time by the closing time of the project task that was accepted last
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

--Find out on all projects which employees on the project do not have unclosed tasks
select w.* 
from Project p
inner join Task t on p.id=t.idProject
inner join TaskStatusLog tsl on t.idTaskstatuslog=tsl.id and tsl.idTaskstatus=4
inner join Workers w on t.id_worker=w.id

-- Assign a task (by title) to transfer the project to an employee with a minimum number of tasks performed by him
declare @pn int = 2 
declare @nz int = 6
update t
set id_worker = (select top 1 t.id_worker from Task t where t.idProject=@pn group by t.idProject, t.id_worker order by count(t.id_worker))
from Task t
where t.idProject=@pn and t.id=@nz