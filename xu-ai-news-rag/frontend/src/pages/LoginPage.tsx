import React, { useState } from 'react'
import { Button, Card, Form, Input, message, Space } from 'antd'
import api from '../api/client'

export default function LoginPage() {
  const [loading, setLoading] = useState(false)

  const onRegister = async (values: any) => {
    setLoading(true)
    try {
      const { data } = await api.post('/api/auth/register', values)
      localStorage.setItem('token', data.token)
      message.success('注册成功，已登录')
    } catch (e: any) {
      message.error('注册失败')
    } finally {
      setLoading(false)
    }
  }

  const onLogin = async (values: any) => {
    setLoading(true)
    try {
      const { data } = await api.post('/api/auth/login', values)
      localStorage.setItem('token', data.token)
      message.success('登录成功')
    } catch (e: any) {
      message.error('登录失败')
    } finally {
      setLoading(false)
    }
  }

  return (
    <Space direction="vertical" style={{ width: 360 }}>
      <Card title="注册" size="small">
        <Form layout="vertical" onFinish={onRegister}>
          <Form.Item name="username" label="用户名" rules={[{ required: true }]}>
            <Input />
          </Form.Item>
          <Form.Item name="email" label="邮箱">
            <Input />
          </Form.Item>
          <Form.Item name="password" label="密码" rules={[{ required: true }]}>
            <Input.Password />
          </Form.Item>
          <Button type="primary" htmlType="submit" loading={loading}>注册并登录</Button>
        </Form>
      </Card>

      <Card title="登录" size="small">
        <Form layout="vertical" onFinish={onLogin}>
          <Form.Item name="username" label="用户名" rules={[{ required: true }]}>
            <Input />
          </Form.Item>
          <Form.Item name="password" label="密码" rules={[{ required: true }]}>
            <Input.Password />
          </Form.Item>
          <Button type="primary" htmlType="submit" loading={loading}>登录</Button>
        </Form>
      </Card>
    </Space>
  )
}
