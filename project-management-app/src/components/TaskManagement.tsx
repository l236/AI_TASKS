import React, { useState } from 'react';
import {
  Box,
  Card,
  CardContent,
  Typography,
  Button,
  Chip,
  Dialog,
  DialogTitle,
  DialogContent,
  DialogActions,
  TextField,
  FormControl,
  InputLabel,
  Select,
  MenuItem,
  LinearProgress,
  Tooltip,
  IconButton
} from '@mui/material';
import {
  Add,
  Edit,
  Delete,
  Visibility,
  Assignment,
  Schedule,
  Person
} from '@mui/icons-material';
import { DataGrid, GridColDef, GridActionsCellItem } from '@mui/x-data-grid';
import { Task, TaskStatus, Priority } from '../types';
import { mockTasks, mockUsers, mockProjects } from '../data/mockData';

const TaskManagement: React.FC = () => {
  const [tasks, setTasks] = useState<Task[]>(mockTasks);
  const [openDialog, setOpenDialog] = useState(false);
  const [editingTask, setEditingTask] = useState<Task | null>(null);
  const [newTask, setNewTask] = useState<Partial<Task>>({
    title: '',
    description: '',
    status: TaskStatus.TODO,
    assignee: '',
    priority: Priority.MEDIUM,
    plannedStartDate: new Date(),
    plannedEndDate: new Date(),
    progress: 0,
    dependencies: [],
    notes: ''
  });

  const getStatusColor = (status: TaskStatus) => {
    switch (status) {
      case TaskStatus.TODO:
        return 'default';
      case TaskStatus.IN_PROGRESS:
        return 'primary';
      case TaskStatus.REVIEW:
        return 'warning';
      case TaskStatus.COMPLETED:
        return 'success';
      case TaskStatus.CANCELLED:
        return 'error';
      default:
        return 'default';
    }
  };

  const getStatusText = (status: TaskStatus) => {
    switch (status) {
      case TaskStatus.TODO:
        return '待开始';
      case TaskStatus.IN_PROGRESS:
        return '进行中';
      case TaskStatus.REVIEW:
        return '待审核';
      case TaskStatus.COMPLETED:
        return '已完成';
      case TaskStatus.CANCELLED:
        return '已取消';
      default:
        return status;
    }
  };

  const getPriorityColor = (priority: Priority) => {
    switch (priority) {
      case Priority.LOW:
        return 'success';
      case Priority.MEDIUM:
        return 'warning';
      case Priority.HIGH:
        return 'error';
      case Priority.URGENT:
        return 'error';
      default:
        return 'default';
    }
  };

  const getPriorityText = (priority: Priority) => {
    switch (priority) {
      case Priority.LOW:
        return '低';
      case Priority.MEDIUM:
        return '中';
      case Priority.HIGH:
        return '高';
      case Priority.URGENT:
        return '紧急';
      default:
        return priority;
    }
  };

  const getUserName = (userId: string) => {
    const user = mockUsers.find(u => u.id === userId);
    return user ? user.name : userId;
  };

  const getProjectName = (projectId: string) => {
    const project = mockProjects.find(p => p.id === projectId);
    return project ? project.name : projectId;
  };

  const columns: GridColDef[] = [
    {
      field: 'title',
      headerName: '任务名称',
      width: 250,
      renderCell: (params) => (
        <Box>
          <Typography variant="subtitle2" fontWeight="bold">
            {params.value}
          </Typography>
          <Typography variant="caption" color="textSecondary">
            {getProjectName(params.row.projectId)}
          </Typography>
        </Box>
      )
    },
    {
      field: 'status',
      headerName: '状态',
      width: 100,
      renderCell: (params) => (
        <Chip
          label={getStatusText(params.value)}
          color={getStatusColor(params.value)}
          size="small"
        />
      )
    },
    {
      field: 'priority',
      headerName: '优先级',
      width: 100,
      renderCell: (params) => (
        <Chip
          label={getPriorityText(params.value)}
          color={getPriorityColor(params.value)}
          size="small"
          variant="outlined"
        />
      )
    },
    {
      field: 'assignee',
      headerName: '负责人',
      width: 120,
      renderCell: (params) => getUserName(params.value)
    },
    {
      field: 'plannedEndDate',
      headerName: '截止日期',
      width: 120,
      renderCell: (params) => (
        <Box display="flex" alignItems="center">
          <Schedule fontSize="small" sx={{ mr: 0.5 }} />
          {params.value.toLocaleDateString()}
        </Box>
      )
    },
    {
      field: 'progress',
      headerName: '进度',
      width: 150,
      renderCell: (params) => (
        <Box>
          <LinearProgress
            variant="determinate"
            value={params.value}
            sx={{ mb: 0.5 }}
          />
          <Typography variant="caption">{params.value}%</Typography>
        </Box>
      )
    },
    {
      field: 'actions',
      type: 'actions',
      headerName: '操作',
      width: 150,
      getActions: (params) => [
        <GridActionsCellItem
          icon={<Visibility />}
          label="查看"
          onClick={() => handleViewTask(params.row)}
        />,
        <GridActionsCellItem
          icon={<Edit />}
          label="编辑"
          onClick={() => handleEditTask(params.row)}
        />,
        <GridActionsCellItem
          icon={<Delete />}
          label="删除"
          onClick={() => handleDeleteTask(params.row.id)}
        />
      ]
    }
  ];

  const handleAddTask = () => {
    setEditingTask(null);
    setNewTask({
      title: '',
      description: '',
      status: TaskStatus.TODO,
      assignee: '',
      priority: Priority.MEDIUM,
      plannedStartDate: new Date(),
      plannedEndDate: new Date(),
      progress: 0,
      dependencies: [],
      notes: ''
    });
    setOpenDialog(true);
  };

  const handleEditTask = (task: Task) => {
    setEditingTask(task);
    setNewTask(task);
    setOpenDialog(true);
  };

  const handleViewTask = (task: Task) => {
    console.log('查看任务:', task);
  };

  const handleDeleteTask = (taskId: string) => {
    if (window.confirm('确定要删除这个任务吗？')) {
      setTasks(tasks.filter(t => t.id !== taskId));
    }
  };

  const handleSaveTask = () => {
    if (editingTask) {
      setTasks(tasks.map(t => 
        t.id === editingTask.id 
          ? { ...newTask, id: editingTask.id, updatedAt: new Date() } as Task
          : t
      ));
    } else {
      const task: Task = {
        ...newTask,
        id: 't' + Date.now(),
        projectId: mockProjects[0].id, // 默认分配到第一个项目
        createdAt: new Date(),
        updatedAt: new Date()
      } as Task;
      setTasks([...tasks, task]);
    }
    setOpenDialog(false);
  };

  // 统计信息
  const stats = {
    total: tasks.length,
    completed: tasks.filter(t => t.status === TaskStatus.COMPLETED).length,
    inProgress: tasks.filter(t => t.status === TaskStatus.IN_PROGRESS).length,
    overdue: tasks.filter(t => t.plannedEndDate < new Date() && t.status !== TaskStatus.COMPLETED).length
  };

  return (
    <Box sx={{ flexGrow: 1, p: 3 }}>
      <Box display="flex" justifyContent="space-between" alignItems="center" mb={3}>
        <Typography variant="h4">
          任务管理
        </Typography>
        <Button
          variant="contained"
          startIcon={<Add />}
          onClick={handleAddTask}
        >
          新建任务
        </Button>
      </Box>

      {/* 统计卡片 */}
      <Box sx={{ display: 'flex', flexWrap: 'wrap', gap: 2, mb: 3 }}>
        <Box sx={{ flex: '1 1 200px', minWidth: '200px' }}>
          <Card>
            <CardContent>
              <Typography color="textSecondary" gutterBottom>
                总任务数
              </Typography>
              <Typography variant="h5">
                {stats.total}
              </Typography>
            </CardContent>
          </Card>
        </Box>
        <Box sx={{ flex: '1 1 200px', minWidth: '200px' }}>
          <Card>
            <CardContent>
              <Typography color="textSecondary" gutterBottom>
                已完成
              </Typography>
              <Typography variant="h5" color="success.main">
                {stats.completed}
              </Typography>
            </CardContent>
          </Card>
        </Box>
        <Box sx={{ flex: '1 1 200px', minWidth: '200px' }}>
          <Card>
            <CardContent>
              <Typography color="textSecondary" gutterBottom>
                进行中
              </Typography>
              <Typography variant="h5" color="primary.main">
                {stats.inProgress}
              </Typography>
            </CardContent>
          </Card>
        </Box>
        <Box sx={{ flex: '1 1 200px', minWidth: '200px' }}>
          <Card>
            <CardContent>
              <Typography color="textSecondary" gutterBottom>
                逾期任务
              </Typography>
              <Typography variant="h5" color="error.main">
                {stats.overdue}
              </Typography>
            </CardContent>
          </Card>
        </Box>
      </Box>

      <Card>
        <DataGrid
          rows={tasks}
          columns={columns}
          pageSizeOptions={[5, 10, 25]}
          initialState={{
            pagination: {
              paginationModel: { page: 0, pageSize: 10 }
            }
          }}
          sx={{ height: 600, border: 0 }}
        />
      </Card>

      {/* 添加/编辑任务对话框 */}
      <Dialog open={openDialog} onClose={() => setOpenDialog(false)} maxWidth="md" fullWidth>
        <DialogTitle>
          {editingTask ? '编辑任务' : '新建任务'}
        </DialogTitle>
        <DialogContent>
          <Box sx={{ display: 'flex', flexWrap: 'wrap', gap: 2, mt: 1 }}>
            <Box sx={{ flex: '1 1 100%' }}>
              <TextField
                fullWidth
                label="任务标题"
                value={newTask.title}
                onChange={(e) => setNewTask({ ...newTask, title: e.target.value })}
              />
            </Box>
            <Box sx={{ flex: '1 1 100%' }}>
              <TextField
                fullWidth
                label="任务描述"
                multiline
                rows={3}
                value={newTask.description}
                onChange={(e) => setNewTask({ ...newTask, description: e.target.value })}
              />
            </Box>
            <Box sx={{ flex: '1 1 300px', minWidth: '250px' }}>
              <FormControl fullWidth>
                <InputLabel>状态</InputLabel>
                <Select
                  value={newTask.status}
                  onChange={(e) => setNewTask({ ...newTask, status: e.target.value as TaskStatus })}
                >
                  <MenuItem value={TaskStatus.TODO}>待开始</MenuItem>
                  <MenuItem value={TaskStatus.IN_PROGRESS}>进行中</MenuItem>
                  <MenuItem value={TaskStatus.REVIEW}>待审核</MenuItem>
                  <MenuItem value={TaskStatus.COMPLETED}>已完成</MenuItem>
                  <MenuItem value={TaskStatus.CANCELLED}>已取消</MenuItem>
                </Select>
              </FormControl>
            </Box>
            <Box sx={{ flex: '1 1 300px', minWidth: '250px' }}>
              <FormControl fullWidth>
                <InputLabel>优先级</InputLabel>
                <Select
                  value={newTask.priority}
                  onChange={(e) => setNewTask({ ...newTask, priority: e.target.value as Priority })}
                >
                  <MenuItem value={Priority.LOW}>低</MenuItem>
                  <MenuItem value={Priority.MEDIUM}>中</MenuItem>
                  <MenuItem value={Priority.HIGH}>高</MenuItem>
                  <MenuItem value={Priority.URGENT}>紧急</MenuItem>
                </Select>
              </FormControl>
            </Box>
            <Box sx={{ flex: '1 1 300px', minWidth: '250px' }}>
              <FormControl fullWidth>
                <InputLabel>负责人</InputLabel>
                <Select
                  value={newTask.assignee}
                  onChange={(e) => setNewTask({ ...newTask, assignee: e.target.value })}
                >
                  {mockUsers.map(user => (
                    <MenuItem key={user.id} value={user.id}>{user.name}</MenuItem>
                  ))}
                </Select>
              </FormControl>
            </Box>
            <Box sx={{ flex: '1 1 300px', minWidth: '250px' }}>
              <TextField
                fullWidth
                label="进度 (%)"
                type="number"
                inputProps={{ min: 0, max: 100 }}
                value={newTask.progress}
                onChange={(e) => setNewTask({ ...newTask, progress: parseInt(e.target.value) || 0 })}
              />
            </Box>
            <Box sx={{ flex: '1 1 300px', minWidth: '250px' }}>
              <TextField
                fullWidth
                label="计划开始时间"
                type="date"
                InputLabelProps={{ shrink: true }}
                value={newTask.plannedStartDate?.toISOString().split('T')[0]}
                onChange={(e) => setNewTask({ 
                  ...newTask, 
                  plannedStartDate: new Date(e.target.value) 
                })}
              />
            </Box>
            <Box sx={{ flex: '1 1 300px', minWidth: '250px' }}>
              <TextField
                fullWidth
                label="计划结束时间"
                type="date"
                InputLabelProps={{ shrink: true }}
                value={newTask.plannedEndDate?.toISOString().split('T')[0]}
                onChange={(e) => setNewTask({ 
                  ...newTask, 
                  plannedEndDate: new Date(e.target.value) 
                })}
              />
            </Box>
            <Box sx={{ flex: '1 1 100%' }}>
              <TextField
                fullWidth
                label="备注"
                multiline
                rows={2}
                value={newTask.notes}
                onChange={(e) => setNewTask({ ...newTask, notes: e.target.value })}
              />
            </Box>
          </Box>
        </DialogContent>
        <DialogActions>
          <Button onClick={() => setOpenDialog(false)}>取消</Button>
          <Button onClick={handleSaveTask} variant="contained">保存</Button>
        </DialogActions>
      </Dialog>
    </Box>
  );
};

export default TaskManagement;
