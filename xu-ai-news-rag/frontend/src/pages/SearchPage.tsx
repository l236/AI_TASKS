import React, { useState } from 'react'
import { Button, Card, Form, Input, List, message, Typography } from 'antd'
import api from '../api/client'

interface SearchHit {
  id?: string
  text?: string
  metadata?: any
  score?: number
  source?: string
  summary?: string
  results?: { title: string; url: string }[]
}

export default function SearchPage() {
  const [loading, setLoading] = useState(false)
  const [hits, setHits] = useState<SearchHit[]>([])

  const onSearch = async (values: any) => {
    setLoading(true)
    try {
      const { data } = await api.post('/api/rag/search', { query: values.query, topK: 5 })
      setHits(data)
    } catch (e) {
      message.error('搜索失败')
    } finally {
      setLoading(false)
    }
  }

  return (
    <Card title="检索">
      <Form layout="inline" onFinish={onSearch}>
        <Form.Item name="query" rules={[{ required: true, message: '请输入查询词' }]}>
          <Input placeholder="例如：OpenAI 新模型" style={{ width: 360 }} />
        </Form.Item>
        <Button type="primary" htmlType="submit" loading={loading}>搜索</Button>
      </Form>

      <List
        style={{ marginTop: 16 }}
        bordered
        dataSource={hits}
        renderItem={(item) => (
          <List.Item>
            {item.source === 'baidu_fallback' ? (
              <div>
                <Typography.Text strong>兜底总结：</Typography.Text>
                <div style={{ whiteSpace: 'pre-wrap' }}>{item.summary || '（无）'}</div>
                <div style={{ marginTop: 8 }}>
                  <Typography.Text type="secondary">参考：</Typography.Text>
                  <ul>
                    {(item.results || []).map((r, idx) => (
                      <li key={idx}><a href={r.url} target="_blank" rel="noreferrer">{r.title}</a></li>
                    ))}
                  </ul>
                </div>
              </div>
            ) : (
              <div>
                <div style={{ marginBottom: 4 }}>
                  <Typography.Text strong>匹配内容：</Typography.Text>
                  <div>{item.text}</div>
                </div>
                {typeof item.score === 'number' && (
                  <Typography.Text type="secondary">score: {item.score.toFixed(3)}</Typography.Text>
                )}
              </div>
            )}
          </List.Item>
        )}
      />
    </Card>
  )
}
