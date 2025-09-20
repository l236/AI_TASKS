import { Project, Task, User, Alert, ProjectStatus, TaskStatus, Priority, UserRole, AlertType } from '../types';

export const mockUsers: User[] = [
  {
    id: '1',
    name: '张三',
    email: 'zhangsan@company.com',
    role: UserRole.MANAGER,
    department: '技术部'
  },
  {
    id: '2',
    name: '李四',
    email: 'lisi@company.com',
    role: UserRole.MEMBER,
    department: '技术部'
  },
  {
    id: '3',
    name: '王五',
    email: 'wangwu@company.com',
    role: UserRole.MEMBER,
    department: '技术部'
  },
  {
    id: '4',
    name: '赵六',
    email: 'zhaoliu@company.com',
    role: UserRole.MEMBER,
    department: '产品部'
  }
];

export const mockProjects: Project[] = [
  {
    id: '1',
    name: '电商平台升级项目',
    department: '技术部',
    status: ProjectStatus.IN_PROGRESS,
    manager: '1',
    members: ['2', '3', '4'],
    plannedStartDate: new Date('2024-01-01'),
    plannedEndDate: new Date('2024-06-30'),
    completionRate: 65,
    milestones: [
      {
        id: 'm1',
        name: '需求分析完成',
        targetDate: new Date('2024-02-15'),
        completed: true,
        completedDate: new Date('2024-02-10')
      },
      {
        id: 'm2',
        name: '系统设计完成',
        targetDate: new Date('2024-03-15'),
        completed: true,
        completedDate: new Date('2024-03-12')
      },
      {
        id: 'm3',
        name: '开发完成',
        targetDate: new Date('2024-05-15'),
        completed: false
      },
      {
        id: 'm4',
        name: '测试完成',
        targetDate: new Date('2024-06-15'),
        completed: false
      }
    ],
    dependencies: [],
    notes: '重点项目，需要重点关注进度',
    createdAt: new Date('2024-01-01'),
    updatedAt: new Date('2024-04-01')
  },
  {
    id: '2',
    name: '移动端APP开发',
    department: '技术部',
    status: ProjectStatus.PLANNING,
    manager: '1',
    members: ['2', '3'],
    plannedStartDate: new Date('2024-04-01'),
    plannedEndDate: new Date('2024-08-31'),
    completionRate: 15,
    milestones: [
      {
        id: 'm5',
        name: 'UI设计完成',
        targetDate: new Date('2024-04-30'),
        completed: false
      },
      {
        id: 'm6',
        name: '开发完成',
        targetDate: new Date('2024-07-31'),
        completed: false
      }
    ],
    dependencies: ['1'],
    notes: '依赖电商平台升级项目完成后开始',
    createdAt: new Date('2024-03-15'),
    updatedAt: new Date('2024-03-15')
  }
];

export const mockTasks: Task[] = [
  {
    id: 't1',
    projectId: '1',
    title: '用户模块开发',
    description: '开发用户注册、登录、个人中心等功能',
    status: TaskStatus.IN_PROGRESS,
    assignee: '2',
    priority: Priority.HIGH,
    plannedStartDate: new Date('2024-03-01'),
    plannedEndDate: new Date('2024-03-31'),
    actualStartDate: new Date('2024-03-01'),
    progress: 80,
    dependencies: [],
    createdAt: new Date('2024-03-01'),
    updatedAt: new Date('2024-04-01')
  },
  {
    id: 't2',
    projectId: '1',
    title: '订单模块开发',
    description: '开发订单创建、支付、状态管理等功能',
    status: TaskStatus.TODO,
    assignee: '3',
    priority: Priority.HIGH,
    plannedStartDate: new Date('2024-04-01'),
    plannedEndDate: new Date('2024-04-30'),
    progress: 0,
    dependencies: ['t1'],
    createdAt: new Date('2024-03-01'),
    updatedAt: new Date('2024-03-01')
  },
  {
    id: 't3',
    projectId: '1',
    title: '商品模块开发',
    description: '开发商品展示、搜索、分类等功能',
    status: TaskStatus.IN_PROGRESS,
    assignee: '4',
    priority: Priority.MEDIUM,
    plannedStartDate: new Date('2024-03-15'),
    plannedEndDate: new Date('2024-04-15'),
    actualStartDate: new Date('2024-03-15'),
    progress: 50,
    dependencies: [],
    createdAt: new Date('2024-03-01'),
    updatedAt: new Date('2024-04-01')
  },
  {
    id: 't4',
    projectId: '2',
    title: 'APP界面设计',
    description: '设计移动端APP的用户界面',
    status: TaskStatus.TODO,
    assignee: '4',
    priority: Priority.MEDIUM,
    plannedStartDate: new Date('2024-04-01'),
    plannedEndDate: new Date('2024-04-30'),
    progress: 0,
    dependencies: [],
    createdAt: new Date('2024-03-15'),
    updatedAt: new Date('2024-03-15')
  }
];

export const mockAlerts: Alert[] = [
  {
    id: 'a1',
    type: AlertType.DEADLINE_WARNING,
    title: '任务即将到期',
    message: '订单模块开发任务将于3天后到期，请及时完成',
    taskId: 't2',
    recipientIds: ['3'],
    isRead: false,
    createdAt: new Date('2024-04-01')
  },
  {
    id: 'a2',
    type: AlertType.RISK_WARNING,
    title: '项目延期风险',
    message: '电商平台升级项目存在延期风险，建议调整资源分配',
    projectId: '1',
    recipientIds: ['1'],
    isRead: false,
    createdAt: new Date('2024-04-01')
  }
];
