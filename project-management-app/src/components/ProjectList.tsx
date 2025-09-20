import React, { useState } from 'react';
import {
  Box,
  Card,
  CardContent,
  Typography,
  Button,
  Chip,
  LinearProgress,
  Dialog,
  DialogTitle,
  DialogContent,
  DialogActions,
  TextField,
  FormControl,
  InputLabel,
  Select,
  MenuItem,
  IconButton,
  Tooltip
} from '@mui/material';
import {
  Add,
  Edit,
  Delete,
  Visibility,
  People,
  Schedule,
  Assignment
} from '@mui/icons-material';
import { DataGrid, GridColDef, GridActionsCellItem } from '@mui/x-data-grid';
import { Project, ProjectStatus } from '../types';
import { mockProjects, mockUsers } from '../data/mockData';

const ProjectList: React.FC = () => {
  const [projects, setProjects] = useState<Project[]>(mockProjects);
  const [openDialog, setOpenDialog] = useState(false);
  const [editingProject, setEditingProject] = useState<Project | null>(null);
  const [newProject, setNewProject] = useState<Partial<Project>>({
    name: '',
    department: '',
    status: ProjectStatus.PLANNING,
    manager: '',
    members: [],
    plannedStartDate: new Date(),
    plannedEndDate: new Date(),
    completionRate: 0,
    milestones: [],
    dependencies: [],
    notes: ''
  });

  const getStatusColor = (status: ProjectStatus) => {
    switch (status) {
      case ProjectStatus.PLANNING:
        return 'default';
      case ProjectStatus.IN_PROGRESS:
        return 'primary';
      case ProjectStatus.ON_HOLD:
        return 'warning';
      case ProjectStatus.COMPLETED:
        return 'success';
      case ProjectStatus.CANCELLED:
        return 'error';
      default:
        return 'default';
    }
  };

  const getStatusText = (status: ProjectStatus) => {
    switch (status) {
      case ProjectStatus.PLANNING:
        return '规划中';
      case ProjectStatus.IN_PROGRESS:
        return '进行中';
      case ProjectStatus.ON_HOLD:
        return '暂停';
      case ProjectStatus.COMPLETED:
        return '已完成';
      case ProjectStatus.CANCELLED:
        return '已取消';
      default:
        return status;
    }
  };

  const getUserName = (userId: string) => {
    const user = mockUsers.find(u => u.id === userId);
    return user ? user.name : userId;
  };

  const columns: GridColDef[] = [
    {
      field: 'name',
      headerName: '项目名称',
      width: 200,
      renderCell: (params) => (
        <Box>
          <Typography variant="subtitle2" fontWeight="bold">
            {params.value}
          </Typography>
          <Typography variant="caption" color="textSecondary">
            {params.row.department}
          </Typography>
        </Box>
      )
    },
    {
      field: 'status',
      headerName: '状态',
      width: 120,
      renderCell: (params) => (
        <Chip
          label={getStatusText(params.value)}
          color={getStatusColor(params.value)}
          size="small"
        />
      )
    },
    {
      field: 'manager',
      headerName: '负责人',
      width: 120,
      renderCell: (params) => getUserName(params.value)
    },
    {
      field: 'members',
      headerName: '成员数',
      width: 100,
      renderCell: (params) => (
        <Box display="flex" alignItems="center">
          <People fontSize="small" sx={{ mr: 0.5 }} />
          {params.value.length}
        </Box>
      )
    },
    {
      field: 'plannedEndDate',
      headerName: '计划结束',
      width: 120,
      renderCell: (params) => params.value.toLocaleDateString()
    },
    {
      field: 'completionRate',
      headerName: '完成率',
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
          onClick={() => handleViewProject(params.row)}
        />,
        <GridActionsCellItem
          icon={<Edit />}
          label="编辑"
          onClick={() => handleEditProject(params.row)}
        />,
        <GridActionsCellItem
          icon={<Delete />}
          label="删除"
          onClick={() => handleDeleteProject(params.row.id)}
        />
      ]
    }
  ];

  const handleAddProject = () => {
    setEditingProject(null);
    setNewProject({
      name: '',
      department: '',
      status: ProjectStatus.PLANNING,
      manager: '',
      members: [],
      plannedStartDate: new Date(),
      plannedEndDate: new Date(),
      completionRate: 0,
      milestones: [],
      dependencies: [],
      notes: ''
    });
    setOpenDialog(true);
  };

  const handleEditProject = (project: Project) => {
    setEditingProject(project);
    setNewProject(project);
    setOpenDialog(true);
  };

  const handleViewProject = (project: Project) => {
    // 这里可以导航到项目详情页面
    console.log('查看项目:', project);
  };

  const handleDeleteProject = (projectId: string) => {
    if (window.confirm('确定要删除这个项目吗？')) {
      setProjects(projects.filter(p => p.id !== projectId));
    }
  };

  const handleSaveProject = () => {
    if (editingProject) {
      // 编辑现有项目
      setProjects(projects.map(p => 
        p.id === editingProject.id 
          ? { ...newProject, id: editingProject.id, updatedAt: new Date() } as Project
          : p
      ));
    } else {
      // 添加新项目
      const project: Project = {
        ...newProject,
        id: Date.now().toString(),
        createdAt: new Date(),
        updatedAt: new Date()
      } as Project;
      setProjects([...projects, project]);
    }
    setOpenDialog(false);
  };

  return (
    <Box sx={{ flexGrow: 1, p: 3 }}>
      <Box display="flex" justifyContent="space-between" alignItems="center" mb={3}>
        <Typography variant="h4">
          项目管理
        </Typography>
        <Button
          variant="contained"
          startIcon={<Add />}
          onClick={handleAddProject}
        >
          新建项目
        </Button>
      </Box>

      <Card>
        <DataGrid
          rows={projects}
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

      {/* 添加/编辑项目对话框 */}
      <Dialog open={openDialog} onClose={() => setOpenDialog(false)} maxWidth="md" fullWidth>
        <DialogTitle>
          {editingProject ? '编辑项目' : '新建项目'}
        </DialogTitle>
        <DialogContent>
          <Box sx={{ display: 'flex', flexWrap: 'wrap', gap: 2, mt: 1 }}>
            <Box sx={{ flex: '1 1 300px', minWidth: '250px' }}>
              <TextField
                fullWidth
                label="项目名称"
                value={newProject.name}
                onChange={(e) => setNewProject({ ...newProject, name: e.target.value })}
              />
            </Box>
            <Box sx={{ flex: '1 1 300px', minWidth: '250px' }}>
              <TextField
                fullWidth
                label="部门"
                value={newProject.department}
                onChange={(e) => setNewProject({ ...newProject, department: e.target.value })}
              />
            </Box>
            <Box sx={{ flex: '1 1 300px', minWidth: '250px' }}>
              <FormControl fullWidth>
                <InputLabel>状态</InputLabel>
                <Select
                  value={newProject.status}
                  onChange={(e) => setNewProject({ ...newProject, status: e.target.value as ProjectStatus })}
                >
                  <MenuItem value={ProjectStatus.PLANNING}>规划中</MenuItem>
                  <MenuItem value={ProjectStatus.IN_PROGRESS}>进行中</MenuItem>
                  <MenuItem value={ProjectStatus.ON_HOLD}>暂停</MenuItem>
                  <MenuItem value={ProjectStatus.COMPLETED}>已完成</MenuItem>
                  <MenuItem value={ProjectStatus.CANCELLED}>已取消</MenuItem>
                </Select>
              </FormControl>
            </Box>
            <Box sx={{ flex: '1 1 300px', minWidth: '250px' }}>
              <FormControl fullWidth>
                <InputLabel>负责人</InputLabel>
                <Select
                  value={newProject.manager}
                  onChange={(e) => setNewProject({ ...newProject, manager: e.target.value })}
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
                label="计划开始时间"
                type="date"
                InputLabelProps={{ shrink: true }}
                value={newProject.plannedStartDate?.toISOString().split('T')[0]}
                onChange={(e) => setNewProject({ 
                  ...newProject, 
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
                value={newProject.plannedEndDate?.toISOString().split('T')[0]}
                onChange={(e) => setNewProject({ 
                  ...newProject, 
                  plannedEndDate: new Date(e.target.value) 
                })}
              />
            </Box>
            <Box sx={{ flex: '1 1 100%' }}>
              <TextField
                fullWidth
                label="备注"
                multiline
                rows={3}
                value={newProject.notes}
                onChange={(e) => setNewProject({ ...newProject, notes: e.target.value })}
              />
            </Box>
          </Box>
        </DialogContent>
        <DialogActions>
          <Button onClick={() => setOpenDialog(false)}>取消</Button>
          <Button onClick={handleSaveProject} variant="contained">保存</Button>
        </DialogActions>
      </Dialog>
    </Box>
  );
};

export default ProjectList;
