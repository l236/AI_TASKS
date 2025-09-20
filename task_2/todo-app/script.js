// 待办事项应用
class TodoApp {
    constructor() {
        this.todos = [];
        this.currentFilter = 'all';
        this.init();
    }

    // 初始化应用
    init() {
        this.loadTodos();
        this.bindEvents();
        this.render();
    }

    // 绑定事件监听器
    bindEvents() {
        // 添加待办事项
        document.getElementById('todo-form').addEventListener('submit', (e) => {
            e.preventDefault();
            this.addTodo();
        });

        // 过滤器按钮
        document.querySelectorAll('.filter-btn').forEach(btn => {
            btn.addEventListener('click', (e) => {
                this.setFilter(e.target.dataset.filter);
            });
        });

        // 清除已完成
        document.getElementById('clear-completed').addEventListener('click', () => {
            this.clearCompleted();
        });

        // 清除全部
        document.getElementById('clear-all').addEventListener('click', () => {
            this.clearAll();
        });
    }

    // 添加待办事项
    addTodo() {
        const input = document.getElementById('todo-input');
        const text = input.value.trim();
        
        if (text === '') {
            alert('请输入待办事项内容！');
            return;
        }

        const todo = {
            id: Date.now(),
            text: text,
            completed: false,
            createdAt: new Date().toISOString()
        };

        this.todos.push(todo);
        input.value = '';
        this.saveTodos();
        this.render();
    }

    // 标记完成/未完成
    toggleTodo(id) {
        const todo = this.todos.find(t => t.id === id);
        if (todo) {
            todo.completed = !todo.completed;
            this.saveTodos();
            this.render();
        }
    }

    // 删除待办事项
    deleteTodo(id) {
        if (confirm('确定要删除这个待办事项吗？')) {
            this.todos = this.todos.filter(t => t.id !== id);
            this.saveTodos();
            this.render();
        }
    }

    // 设置过滤器
    setFilter(filter) {
        this.currentFilter = filter;
        
        // 更新按钮状态
        document.querySelectorAll('.filter-btn').forEach(btn => {
            btn.classList.remove('active');
        });
        document.querySelector(`[data-filter="${filter}"]`).classList.add('active');
        
        this.render();
    }

    // 获取过滤后的待办事项
    getFilteredTodos() {
        switch (this.currentFilter) {
            case 'active':
                return this.todos.filter(todo => !todo.completed);
            case 'completed':
                return this.todos.filter(todo => todo.completed);
            default:
                return this.todos;
        }
    }

    // 清除已完成的待办事项
    clearCompleted() {
        const completedCount = this.todos.filter(todo => todo.completed).length;
        if (completedCount === 0) {
            alert('没有已完成的待办事项！');
            return;
        }
        
        if (confirm(`确定要清除 ${completedCount} 个已完成的待办事项吗？`)) {
            this.todos = this.todos.filter(todo => !todo.completed);
            this.saveTodos();
            this.render();
        }
    }

    // 清除全部待办事项
    clearAll() {
        if (this.todos.length === 0) {
            alert('没有待办事项！');
            return;
        }
        
        if (confirm(`确定要清除全部 ${this.todos.length} 个待办事项吗？`)) {
            this.todos = [];
            this.saveTodos();
            this.render();
        }
    }

    // 渲染待办事项列表
    render() {
        const todoList = document.getElementById('todo-list');
        const filteredTodos = this.getFilteredTodos();

        if (filteredTodos.length === 0) {
            todoList.innerHTML = this.getEmptyStateHTML();
            return;
        }

        todoList.innerHTML = filteredTodos.map(todo => this.createTodoHTML(todo)).join('');
        
        // 绑定每个待办事项的事件
        filteredTodos.forEach(todo => {
            const todoElement = document.getElementById(`todo-${todo.id}`);
            if (todoElement) {
                // 完成按钮
                const completeBtn = todoElement.querySelector('.complete-btn');
                completeBtn.addEventListener('click', () => this.toggleTodo(todo.id));
                
                // 删除按钮
                const deleteBtn = todoElement.querySelector('.delete-btn');
                deleteBtn.addEventListener('click', () => this.deleteTodo(todo.id));
            }
        });
    }

    // 创建待办事项HTML
    createTodoHTML(todo) {
        return `
            <li id="todo-${todo.id}" class="todo-item ${todo.completed ? 'completed' : ''}">
                <span class="todo-text">${this.escapeHtml(todo.text)}</span>
                <div class="todo-actions">
                    <button class="todo-btn complete-btn" ${todo.completed ? 'disabled' : ''}>
                        ${todo.completed ? '已完成' : '完成'}
                    </button>
                    <button class="todo-btn delete-btn">删除</button>
                </div>
            </li>
        `;
    }

    // 获取空状态HTML
    getEmptyStateHTML() {
        const messages = {
            all: '还没有待办事项，添加一个吧！',
            active: '没有未完成的待办事项！',
            completed: '没有已完成的待办事项！'
        };
        
        return `
            <li class="empty-state">
                <h3>📝</h3>
                <p>${messages[this.currentFilter]}</p>
            </li>
        `;
    }

    // HTML转义，防止XSS攻击
    escapeHtml(text) {
        const div = document.createElement('div');
        div.textContent = text;
        return div.innerHTML;
    }

    // 保存到LocalStorage
    saveTodos() {
        try {
            localStorage.setItem('todos', JSON.stringify(this.todos));
        } catch (error) {
            console.error('保存待办事项失败:', error);
            alert('保存失败，请检查浏览器存储空间！');
        }
    }

    // 从LocalStorage加载
    loadTodos() {
        try {
            const saved = localStorage.getItem('todos');
            if (saved) {
                this.todos = JSON.parse(saved);
            }
        } catch (error) {
            console.error('加载待办事项失败:', error);
            this.todos = [];
        }
    }
}

// 页面加载完成后初始化应用
document.addEventListener('DOMContentLoaded', () => {
    new TodoApp();
});

// 添加一些示例数据（仅在首次访问时）
if (!localStorage.getItem('todos')) {
    const sampleTodos = [
        {
            id: 1,
            text: '学习JavaScript基础',
            completed: false,
            createdAt: new Date().toISOString()
        },
        {
            id: 2,
            text: '完成项目任务',
            completed: true,
            createdAt: new Date().toISOString()
        },
        {
            id: 3,
            text: '阅读技术文档',
            completed: false,
            createdAt: new Date().toISOString()
        }
    ];
    localStorage.setItem('todos', JSON.stringify(sampleTodos));
}
