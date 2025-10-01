import React, { useEffect, useState } from 'react'
import { Card, List, message } from 'antd'
import api from '../api/client'

interface Item { keyword: string; count: number }

export default function KeywordsPage() {
  const [data, setData] = useState<Item[]>([])

  useEffect(() => {
    (async () => {
      try {
        const { data } = await api.get<Item[]>('/api/analytics/keywords/top10')
        setData(data)
      } catch (e) {
        message.error('加载失败')
      }
    })()
  }, [])

  return (
    <Card title="关键词 Top10">
      <List
        bordered
        dataSource={data}
        renderItem={(it) => (
          <List.Item>
            <div style={{ display: 'flex', justifyContent: 'space-between', width: '100%' }}>
              <span>{it.keyword}</span>
              <span>{it.count}</span>
            </div>
          </List.Item>
        )}
      />
    </Card>
  )
}
