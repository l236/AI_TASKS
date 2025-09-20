import React from 'react';
import {
  Box,
  Card,
  CardContent,
  Typography,
  Paper,
  List,
  ListItem,
  ListItemText,
  ListItemIcon,
  Chip,
  Avatar
} from '@mui/material';
import {
  TrendingUp,
  Assignment,
  People,
  Warning,
  CheckCircle,
  Schedule
} from '@mui/icons-material';
import { LineChart, Line, XAxis, YAxis, CartesianGrid, Tooltip, ResponsiveContainer, BarChart, Bar } from 'recharts';
import { mockProjects, mockTasks, mockAlerts } from '../data/mockData';

const Dashboard: React.FC = () => {
  // 计算统计数据
  const stats = {
    totalProjects: mockProjects.length,
    activeProjects: mockProjects.filter(p => p.status === 'in_progress').length,
    completedProjects: mockProjects.filter(p => p.status === 'completed').length,
    totalTasks: mockTasks.length,
    completedTasks: mockTasks.filter(t => t.status === 'completed').length,
    overdueTasks: mockTasks.filter(t => 
      t.plannedEndDate < new Date() && t.status !== 'completed'
    ).length,
    teamMembers: 4
  };

  // 项目进度数据
  const progressData = mockProjects.map(project => ({
    name: project.name.length > 10 ? project.name.substring(0, 10) + '...' : project.name,
    progress: project.completionRate
  }));

  // 任务状态分布数据
  const taskStatusData = [
    { name: '待开始', value: mockTasks.filter(t => t.status === 'todo').length, color: '#f44336' },
    { name: '进行中', value: mockTasks.filter(t => t.status === 'in_progress').length, color: '#ff9800' },
    { name: '待审核', value: mockTasks.filter(t => t.status === 'review').length, color: '#2196f3' },
    { name: '已完成', value: mockTasks.filter(t => t.status === 'completed').length, color: '#4caf50' }
  ];

  // 最近活动数据
  const recentActivities = [
    { id: 1, type: 'task', message: '李四完成了用户模块开发任务', time: '2小时前' },
    { id: 2, type: 'milestone', message: '电商平台升级项目达到系统设计里程碑', time: '1天前' },
    { id: 3, type: 'alert', message: '订单模块开发任务即将到期', time: '2天前' }
  ];

  const StatCard: React.FC<{ title: string; value: number; icon: React.ReactNode; color: string }> = ({ title, value, icon, color }) => (
    <Card>
      <CardContent>
        <Box display="flex" alignItems="center" justifyContent="space-between">
          <Box>
            <Typography color="textSecondary" gutterBottom>
              {title}
            </Typography>
            <Typography variant="h4" component="div">
              {value}
            </Typography>
          </Box>
          <Avatar sx={{ bgcolor: color }}>
            {icon}
          </Avatar>
        </Box>
      </CardContent>
    </Card>
  );

  return (
    <Box sx={{ flexGrow: 1, p: 3 }}>
      <Typography variant="h4" gutterBottom>
        项目仪表板
      </Typography>

      {/* 统计卡片 */}
      <Box sx={{ display: 'flex', flexWrap: 'wrap', gap: 3, mb: 3 }}>
        <Box sx={{ flex: '1 1 200px', minWidth: '200px' }}>
          <StatCard
            title="总项目数"
            value={stats.totalProjects}
            icon={<Assignment />}
            color="#2196f3"
          />
        </Box>
        <Box sx={{ flex: '1 1 200px', minWidth: '200px' }}>
          <StatCard
            title="进行中项目"
            value={stats.activeProjects}
            icon={<TrendingUp />}
            color="#ff9800"
          />
        </Box>
        <Box sx={{ flex: '1 1 200px', minWidth: '200px' }}>
          <StatCard
            title="完成任务"
            value={stats.completedTasks}
            icon={<CheckCircle />}
            color="#4caf50"
          />
        </Box>
        <Box sx={{ flex: '1 1 200px', minWidth: '200px' }}>
          <StatCard
            title="逾期任务"
            value={stats.overdueTasks}
            icon={<Warning />}
            color="#f44336"
          />
        </Box>
      </Box>

      <Box sx={{ display: 'flex', flexWrap: 'wrap', gap: 3 }}>
        {/* 项目进度图表 */}
        <Box sx={{ flex: '2 1 400px' }}>
          <Card>
            <CardContent>
              <Typography variant="h6" gutterBottom>
                项目进度概览
              </Typography>
              <ResponsiveContainer width="100%" height={300}>
                <BarChart data={progressData}>
                  <CartesianGrid strokeDasharray="3 3" />
                  <XAxis dataKey="name" />
                  <YAxis />
                  <Tooltip />
                  <Bar dataKey="progress" fill="#2196f3" />
                </BarChart>
              </ResponsiveContainer>
            </CardContent>
          </Card>
        </Box>

        {/* 任务状态分布 */}
        <Box sx={{ flex: '1 1 300px' }}>
          <Card>
            <CardContent>
              <Typography variant="h6" gutterBottom>
                任务状态分布
              </Typography>
              <Box>
                {taskStatusData.map((item, index) => (
                  <Box key={index} display="flex" alignItems="center" justifyContent="space-between" mb={1}>
                    <Box display="flex" alignItems="center">
                      <Box
                        sx={{
                          width: 12,
                          height: 12,
                          borderRadius: '50%',
                          backgroundColor: item.color,
                          mr: 1
                        }}
                      />
                      <Typography variant="body2">{item.name}</Typography>
                    </Box>
                    <Chip label={item.value} size="small" />
                  </Box>
                ))}
              </Box>
            </CardContent>
          </Card>
        </Box>

        {/* 最近活动 */}
        <Box sx={{ flex: '1 1 400px' }}>
          <Card>
            <CardContent>
              <Typography variant="h6" gutterBottom>
                最近活动
              </Typography>
              <List>
                {recentActivities.map((activity) => (
                  <ListItem key={activity.id}>
                    <ListItemIcon>
                      {activity.type === 'task' && <Assignment />}
                      {activity.type === 'milestone' && <CheckCircle />}
                      {activity.type === 'alert' && <Warning />}
                    </ListItemIcon>
                    <ListItemText
                      primary={activity.message}
                      secondary={activity.time}
                    />
                  </ListItem>
                ))}
              </List>
            </CardContent>
          </Card>
        </Box>

        {/* 预警信息 */}
        <Box sx={{ flex: '1 1 400px' }}>
          <Card>
            <CardContent>
              <Typography variant="h6" gutterBottom>
                预警信息
              </Typography>
              <List>
                {mockAlerts.map((alert) => (
                  <ListItem key={alert.id}>
                    <ListItemIcon>
                      <Warning color={alert.isRead ? "disabled" : "error"} />
                    </ListItemIcon>
                    <ListItemText
                      primary={alert.title}
                      secondary={alert.message}
                    />
                    <Chip
                      label={alert.type === 'deadline_warning' ? '期限预警' : '风险预警'}
                      color={alert.type === 'deadline_warning' ? 'warning' : 'error'}
                      size="small"
                    />
                  </ListItem>
                ))}
              </List>
            </CardContent>
          </Card>
        </Box>
      </Box>
    </Box>
  );
};

export default Dashboard;
