import React, { useMemo } from 'react'
import { Button, Layout, Menu, theme } from 'antd'
import { Link, Route, Routes, useLocation, useNavigate } from 'react-router-dom'
import LoginPage from './pages/LoginPage'
import SearchPage from './pages/SearchPage'
import UploadPage from './pages/UploadPage'
import KeywordsPage from './pages/KeywordsPage'
import RequireAuth from './components/RequireAuth'

const { Header, Content, Footer, Sider } = Layout

function App() {
  const { token } = theme.useToken()
  const location = useLocation()
  const selectedKeys = useMemo(() => {
    if (location.pathname.startsWith('/search')) return ['search']
    if (location.pathname.startsWith('/upload')) return ['upload']
    if (location.pathname.startsWith('/keywords')) return ['keywords']
    return ['login']
  }, [location.pathname])

  return (
    <Layout style={{ minHeight: '100vh' }}>
      <Sider collapsible>
        <div style={{ color: '#fff', padding: 16, fontWeight: 600 }}>xu-ai-news-rag</div>
        <Menu theme="dark" mode="inline" selectedKeys={selectedKeys}
          items={[
            { key: 'login', label: <Link to="/login">登录/注册</Link> },
            { key: 'search', label: <Link to="/search">检索</Link> },
            { key: 'upload', label: <Link to="/upload">Excel 导入</Link> },
            { key: 'keywords', label: <Link to="/keywords">关键词 Top10</Link> },
          ]}
        />
      </Sider>
      <Layout>
        <Header style={{ background: token.colorBgContainer, display: 'flex', justifyContent: 'flex-end' }}>
          <Button onClick={() => { localStorage.removeItem('token'); window.location.href = '/login' }}>退出登录</Button>
        </Header>
        <Content style={{ margin: '16px' }}>
          <Routes>
            <Route path="/login" element={<LoginPage />} />
            <Route path="/search" element={<RequireAuth><SearchPage /></RequireAuth>} />
            <Route path="/upload" element={<RequireAuth><UploadPage /></RequireAuth>} />
            <Route path="/keywords" element={<RequireAuth><KeywordsPage /></RequireAuth>} />
            <Route path="*" element={<LoginPage />} />
          </Routes>
        </Content>
        <Footer style={{ textAlign: 'center' }}>©{new Date().getFullYear()} xu-ai-news-rag</Footer>
      </Layout>
    </Layout>
  )
}

export default App
