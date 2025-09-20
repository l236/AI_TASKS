import React, { useState } from 'react';
import {
  Box,
  Card,
  CardContent,
  Typography,
  Tabs,
  Tab,
  LinearProgress,
  Chip,
  Paper,
  List,
  ListItem,
  ListItemText,
  ListItemIcon,
  Avatar
} from '@mui/material';
import {
  Timeline,
  PieChart,
  BarChart,
  CheckCircle,
  Schedule,
  Warning
} from '@mui/icons-material';
import {
  LineChart,
  Line,
  AreaChart,
  Area,
  BarChart as RechartsBarChart,
  Bar,
  PieChart as RechartsPieChart,
  Pie,
  Cell,
  XAxis,
  YAxis,
  CartesianGrid,
  Tooltip,
  ResponsiveContainer,
  Legend
} from 'recharts';
import { mockProjects, mockTasks } from '../data/mockData';

interface TabPanelProps {
  children?: React.ReactNode;
  index: number;
  value: number;
}

function TabPanel(props: TabPanelProps) {
  const { children, value, index, ...other } = props;

  return (
    <div
      role="tabpanel"
      hidden={value !== index}
      id={`simple-tabpanel-${index}`}
      aria-labelledby={`simple-tab-${index}`}
      {...other}
    >
      {value === index && (
        <Box sx={{ p: 3 }}>
          {children}
        </Box>
      )}
    </div>
  );
}

const ProgressVisualization: React.FC = () => {
  const [tabValue, setTabValue] = useState(0);

  const handleTabChange = (event: React.SyntheticEvent, newValue: number) => {
    setTabValue(newValue);
  };

  // 项目进度数据
  const projectProgressData = mockProjects.map(project => ({
    name: project.name.length > 8 ? project.name.substring(0, 8) + '...' : project.name,
    progress: project.completionRate,
    planned: 100,
    actual: project.completionRate
  }));

  // 任务状态分布数据
  const taskStatusData = [
    { name: '待开始', value: mockTasks.filter(t => t.status === 'todo').length, color: '#f44336' },
    { name: '进行中', value: mockTasks.filter(t => t.status === 'in_progress').length, color: '#ff9800' },
    { name: '待审核', value: mockTasks.filter(t => t.status === 'review').length, color: '#2196f3' },
    { name: '已完成', value: mockTasks.filter(t => t.status === 'completed').length, color: '#4caf50' }
  ];

  // 团队工作量数据
  const teamWorkloadData = mockProjects.map(project => ({
    name: project.name.length > 6 ? project.name.substring(0, 6) + '...' : project.name,
    members: project.members.length,
    tasks: mockTasks.filter(t => t.projectId === project.id).length,
    completed: mockTasks.filter(t => t.projectId === project.id && t.status === 'completed').length
  }));

  // 时间线数据
  const timelineData = mockProjects.flatMap(project => 
    project.milestones.map(milestone => ({
      id: milestone.id,
      projectName: project.name,
      milestoneName: milestone.name,
      targetDate: milestone.targetDate,
      completed: milestone.completed,
      completedDate: milestone.completedDate
    }))
  ).sort((a, b) => a.targetDate.getTime() - b.targetDate.getTime());

  // 甘特图数据
  const ganttData = mockProjects.map(project => ({
    name: project.name,
    start: project.plannedStartDate,
    end: project.plannedEndDate,
    progress: project.completionRate
  }));

  const COLORS = ['#0088FE', '#00C49F', '#FFBB28', '#FF8042', '#8884D8'];

  return (
    <Box sx={{ flexGrow: 1, p: 3 }}>
      <Typography variant="h4" gutterBottom>
        进度可视化
      </Typography>

      <Box sx={{ borderBottom: 1, borderColor: 'divider', mb: 3 }}>
        <Tabs value={tabValue} onChange={handleTabChange} aria-label="进度可视化标签">
          <Tab icon={<BarChart />} label="项目进度" />
          <Tab icon={<PieChart />} label="任务分布" />
          <Tab icon={<Timeline />} label="时间线" />
          <Tab icon={<Schedule />} label="甘特图" />
        </Tabs>
      </Box>

      <TabPanel value={tabValue} index={0}>
        <Box sx={{ display: 'flex', flexWrap: 'wrap', gap: 3 }}>
          {/* 项目进度对比图 */}
          <Box sx={{ flex: '2 1 500px' }}>
            <Card>
              <CardContent>
                <Typography variant="h6" gutterBottom>
                  项目进度对比
                </Typography>
                <ResponsiveContainer width="100%" height={300}>
                  <RechartsBarChart data={projectProgressData}>
                    <CartesianGrid strokeDasharray="3 3" />
                    <XAxis dataKey="name" />
                    <YAxis />
                    <Tooltip />
                    <Legend />
                    <Bar dataKey="planned" fill="#e0e0e0" name="计划进度" />
                    <Bar dataKey="actual" fill="#2196f3" name="实际进度" />
                  </RechartsBarChart>
                </ResponsiveContainer>
              </CardContent>
            </Card>
          </Box>

          {/* 项目进度列表 */}
          <Box sx={{ flex: '1 1 300px' }}>
            <Card>
              <CardContent>
                <Typography variant="h6" gutterBottom>
                  项目进度详情
                </Typography>
                <List>
                  {mockProjects.map((project, index) => (
                    <ListItem key={project.id}>
                      <ListItemIcon>
                        <Avatar sx={{ bgcolor: COLORS[index % COLORS.length] }}>
                          {project.name.charAt(0)}
                        </Avatar>
                      </ListItemIcon>
                      <ListItemText
                        primary={project.name}
                        secondary={
                          <Box>
                            <LinearProgress
                              variant="determinate"
                              value={project.completionRate}
                              sx={{ mb: 0.5 }}
                            />
                            <Typography variant="caption">
                              {project.completionRate}% 完成
                            </Typography>
                          </Box>
                        }
                      />
                    </ListItem>
                  ))}
                </List>
              </CardContent>
            </Card>
          </Box>

          {/* 团队工作量分析 */}
          <Box sx={{ flex: '1 1 100%' }}>
            <Card>
              <CardContent>
                <Typography variant="h6" gutterBottom>
                  团队工作量分析
                </Typography>
                <ResponsiveContainer width="100%" height={300}>
                  <RechartsBarChart data={teamWorkloadData}>
                    <CartesianGrid strokeDasharray="3 3" />
                    <XAxis dataKey="name" />
                    <YAxis />
                    <Tooltip />
                    <Legend />
                    <Bar dataKey="members" fill="#8884d8" name="团队成员数" />
                    <Bar dataKey="tasks" fill="#82ca9d" name="任务总数" />
                    <Bar dataKey="completed" fill="#ffc658" name="已完成任务" />
                  </RechartsBarChart>
                </ResponsiveContainer>
              </CardContent>
            </Card>
          </Box>
        </Box>
      </TabPanel>

      <TabPanel value={tabValue} index={1}>
        <Box sx={{ display: 'flex', flexWrap: 'wrap', gap: 3 }}>
          {/* 任务状态饼图 */}
          <Box sx={{ flex: '1 1 400px' }}>
            <Card>
              <CardContent>
                <Typography variant="h6" gutterBottom>
                  任务状态分布
                </Typography>
                <ResponsiveContainer width="100%" height={300}>
                  <RechartsPieChart>
                    <Pie
                      data={taskStatusData}
                      cx="50%"
                      cy="50%"
                      labelLine={false}
                      label={({ name, percent }: any) => `${name} ${(percent * 100).toFixed(0)}%`}
                      outerRadius={80}
                      fill="#8884d8"
                      dataKey="value"
                    >
                      {taskStatusData.map((entry, index) => (
                        <Cell key={`cell-${index}`} fill={entry.color} />
                      ))}
                    </Pie>
                    <Tooltip />
                  </RechartsPieChart>
                </ResponsiveContainer>
              </CardContent>
            </Card>
          </Box>

          {/* 任务状态详情 */}
          <Box sx={{ flex: '1 1 400px' }}>
            <Card>
              <CardContent>
                <Typography variant="h6" gutterBottom>
                  任务状态详情
                </Typography>
                <List>
                  {taskStatusData.map((status, index) => (
                    <ListItem key={index}>
                      <ListItemIcon>
                        <Avatar sx={{ bgcolor: status.color, width: 24, height: 24 }}>
                          {status.value}
                        </Avatar>
                      </ListItemIcon>
                      <ListItemText
                        primary={status.name}
                        secondary={`${status.value} 个任务`}
                      />
                      <Chip
                        label={`${((status.value / mockTasks.length) * 100).toFixed(1)}%`}
                        size="small"
                        color="primary"
                        variant="outlined"
                      />
                    </ListItem>
                  ))}
                </List>
              </CardContent>
            </Card>
          </Box>
        </Box>
      </TabPanel>

      <TabPanel value={tabValue} index={2}>
        <Card>
          <CardContent>
            <Typography variant="h6" gutterBottom>
              项目里程碑时间线
            </Typography>
            <List>
              {timelineData.map((item, index) => (
                <ListItem key={item.id}>
                  <ListItemIcon>
                    {item.completed ? (
                      <CheckCircle color="success" />
                    ) : (
                      <Schedule color={item.targetDate < new Date() ? "error" : "primary"} />
                    )}
                  </ListItemIcon>
                  <ListItemText
                    primary={
                      <Box display="flex" alignItems="center" gap={1}>
                        <Typography variant="subtitle1">
                          {item.milestoneName}
                        </Typography>
                        <Chip
                          label={item.projectName}
                          size="small"
                          variant="outlined"
                        />
                        {item.completed && (
                          <Chip
                            label="已完成"
                            size="small"
                            color="success"
                          />
                        )}
                        {!item.completed && item.targetDate < new Date() && (
                          <Chip
                            label="逾期"
                            size="small"
                            color="error"
                          />
                        )}
                      </Box>
                    }
                    secondary={
                      <Box>
                        <Typography variant="body2" color="textSecondary">
                          目标日期: {item.targetDate.toLocaleDateString()}
                        </Typography>
                        {item.completedDate && (
                          <Typography variant="body2" color="textSecondary">
                            完成日期: {item.completedDate.toLocaleDateString()}
                          </Typography>
                        )}
                      </Box>
                    }
                  />
                </ListItem>
              ))}
            </List>
          </CardContent>
        </Card>
      </TabPanel>

      <TabPanel value={tabValue} index={3}>
        <Card>
          <CardContent>
            <Typography variant="h6" gutterBottom>
              项目甘特图
            </Typography>
            <Paper sx={{ p: 2 }}>
              <Box sx={{ display: 'flex', flexDirection: 'column', gap: 2 }}>
                {ganttData.map((project, index) => (
                  <Box sx={{ flex: '1 1 100%' }} key={index}>
                    <Box display="flex" alignItems="center" mb={1}>
                      <Typography variant="body1" sx={{ minWidth: 150, mr: 2 }}>
                        {project.name}
                      </Typography>
                      <Box flexGrow={1} position="relative">
                        <LinearProgress
                          variant="determinate"
                          value={project.progress}
                          sx={{ height: 8, borderRadius: 4 }}
                        />
                        <Typography
                          variant="caption"
                          sx={{
                            position: 'absolute',
                            right: 8,
                            top: -20,
                            color: 'text.secondary'
                          }}
                        >
                          {project.progress}%
                        </Typography>
                      </Box>
                    </Box>
                    <Typography variant="caption" color="textSecondary">
                      {project.start.toLocaleDateString()} - {project.end.toLocaleDateString()}
                    </Typography>
                  </Box>
                ))}
              </Box>
            </Paper>
          </CardContent>
        </Card>
      </TabPanel>
    </Box>
  );
};

export default ProgressVisualization;
