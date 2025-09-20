export interface Project {
  id: string;
  name: string;
  department: string;
  status: ProjectStatus;
  manager: string;
  members: string[];
  plannedStartDate: Date;
  plannedEndDate: Date;
  actualEndDate?: Date;
  duration?: number;
  completionRate: number;
  milestones: Milestone[];
  dependencies: string[];
  notes?: string;
  createdAt: Date;
  updatedAt: Date;
}

export interface Task {
  id: string;
  projectId: string;
  title: string;
  description?: string;
  status: TaskStatus;
  assignee: string;
  priority: Priority;
  plannedStartDate: Date;
  plannedEndDate: Date;
  actualStartDate?: Date;
  actualEndDate?: Date;
  progress: number;
  dependencies: string[];
  notes?: string;
  createdAt: Date;
  updatedAt: Date;
}

export interface Milestone {
  id: string;
  name: string;
  targetDate: Date;
  completed: boolean;
  completedDate?: Date;
}

export interface User {
  id: string;
  name: string;
  email: string;
  role: UserRole;
  department: string;
  avatar?: string;
}

export interface Alert {
  id: string;
  type: AlertType;
  title: string;
  message: string;
  projectId?: string;
  taskId?: string;
  recipientIds: string[];
  isRead: boolean;
  createdAt: Date;
}

export enum ProjectStatus {
  PLANNING = 'planning',
  IN_PROGRESS = 'in_progress',
  ON_HOLD = 'on_hold',
  COMPLETED = 'completed',
  CANCELLED = 'cancelled'
}

export enum TaskStatus {
  TODO = 'todo',
  IN_PROGRESS = 'in_progress',
  REVIEW = 'review',
  COMPLETED = 'completed',
  CANCELLED = 'cancelled'
}

export enum Priority {
  LOW = 'low',
  MEDIUM = 'medium',
  HIGH = 'high',
  URGENT = 'urgent'
}

export enum UserRole {
  ADMIN = 'admin',
  MANAGER = 'manager',
  MEMBER = 'member'
}

export enum AlertType {
  DEADLINE_WARNING = 'deadline_warning',
  OVERDUE = 'overdue',
  MILESTONE_REACHED = 'milestone_reached',
  RISK_WARNING = 'risk_warning',
  TASK_ASSIGNED = 'task_assigned'
}

export interface DashboardStats {
  totalProjects: number;
  activeProjects: number;
  completedProjects: number;
  totalTasks: number;
  completedTasks: number;
  overdueTasks: number;
  teamMembers: number;
}
