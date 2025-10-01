import React, { useState } from 'react'
import { Button, Card, Upload, message } from 'antd'
import type { UploadProps } from 'antd'
import { UploadOutlined } from '@ant-design/icons'
import api from '../api/client'

export default function UploadPage() {
  const [file, setFile] = useState<File | null>(null)
  const [loading, setLoading] = useState(false)

  const props: UploadProps = {
    beforeUpload: (f) => {
      setFile(f)
      return false // prevent auto upload
    },
    maxCount: 1,
    accept: '.xlsx'
  }

  const onUpload = async () => {
    if (!file) return message.warning('请选择 Excel 文件 (.xlsx)')
    const form = new FormData()
    form.append('file', file)
    setLoading(true)
    try {
      const { data } = await api.post('/api/rag/ingest/excel', form, {
        headers: { 'Content-Type': 'multipart/form-data' }
      })
      message.success(`导入成功：${data.success}，失败：${data.failed}`)
    } catch (e) {
      message.error('导入失败')
    } finally {
      setLoading(false)
    }
  }

  return (
    <Card title="Excel 导入">
      <Upload {...props}>
        <Button icon={<UploadOutlined />}>选择 Excel 文件</Button>
      </Upload>
      <div style={{ marginTop: 12 }}>
        <Button type="primary" onClick={onUpload} loading={loading}>上传并导入</Button>
      </div>
      <div style={{ marginTop: 12, color: '#888' }}>
        模板列顺序：sourceName, sourceUrl, title, url, content
      </div>
    </Card>
  )
}
