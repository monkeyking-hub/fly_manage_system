# Sky Wings 航班预订系统 - API 文档

## 概述

Sky Wings 是一个现代化的航班预订系统，提供完整的用户认证、航班搜索、预订、订单管理等功能。本文档详细说明了所有 API 端点的使用方法。

---

## 基础信息

### 服务地址
- **协议**: HTTP/HTTPS
- **基础URL**: `http://localhost:8080/api`
- **数据格式**: JSON

### 通用响应格式

所有API响应遵循统一的格式：

```json
{
  "code": 200,
  "message": "Success message",
  "data": {}
}
```

| 字段 | 类型 | 说明 |
|------|------|------|
| code | Integer | 状态码 (200=成功, 4xx=客户端错误, 5xx=服务器错误) |
| message | String | 信息描述 |
| data | Object/Array | 返回的数据 |

### 常见状态码

| 码 | 含义 |
|----|------|
| 200 | 请求成功 |
| 201 | 资源创建成功 |
| 400 | 请求参数错误 |
| 401 | 未授权/认证失败 |
| 403 | 禁止访问 |
| 404 | 资源不存在 |
| 500 | 服务器内部错误 |

---

## 用户认证 API

### 1. 用户注册

**端点**: `POST /users/register`

**描述**: 创建新用户账户

**请求头**:
```
Content-Type: application/json
```

**请求体**:
```json
{
  "username": "john_doe",
  "email": "john@example.com",
  "password": "SecurePassword123!",
  "phone": "+1234567890",
  "age": 28,
  "gender": "Male",
  "idNumber": "1234567890"
}
```

**请求字段说明**:

| 字段 | 类型 | 必需 | 说明 |
|------|------|------|------|
| username | String | ✓ | 用户名, 3-20字符, 只能包含字母数字和下划线 |
| email | String | ✓ | 邮箱地址, 必须唯一 |
| password | String | ✓ | 密码, 至少8字符, 需包含大小写字母和数字 |
| phone | String | ✓ | 电话号码 |
| age | Integer | ✓ | 年龄, 18-120 |
| gender | String | ✓ | 性别, 枚举值: Male/Female/Other |
| idNumber | String | ✓ | 身份证号 |

**成功响应** (201):
```json
{
  "code": 201,
  "message": "Registration successful",
  "data": {
    "id": 1,
    "username": "john_doe",
    "email": "john@example.com",
    "createdAt": "2024-01-15T10:30:00Z"
  }
}
```

**失败响应** (400/500):
```json
{
  "code": 400,
  "message": "Email already exists",
  "data": null
}
```

---

### 2. 用户登录

**端点**: `POST /users/login`

**描述**: 用户登录, 返回认证token

**请求头**:
```
Content-Type: application/json
```

**请求体**:
```json
{
  "email": "john@example.com",
  "password": "SecurePassword123!"
}
```

**请求字段说明**:

| 字段 | 类型 | 必需 | 说明 |
|------|------|------|------|
| email | String | ✓ | 注册邮箱 |
| password | String | ✓ | 账户密码 |

**成功响应** (200):
```json
{
  "code": 200,
  "message": "Login successful",
  "data": {
    "token": "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9...",
    "userId": 1,
    "username": "john_doe",
    "email": "john@example.com",
    "profile": {
      "phone": "+1234567890",
      "age": 28,
      "gender": "Male",
      "avatar": null
    },
    "expiresIn": 86400
  }
}
```

**失败响应** (401):
```json
{
  "code": 401,
  "message": "Invalid email or password",
  "data": null
}
```

---

### 3. 登出

**端点**: `POST /users/logout`

**描述**: 用户登出

**请求头**:
```
Content-Type: application/json
Authorization: Bearer <token>
```

**成功响应** (200):
```json
{
  "code": 200,
  "message": "Logout successful",
  "data": null
}
```

---

### 4. 获取用户信息

**端点**: `GET /users/profile`

**描述**: 获取当前登录用户的详细信息

**请求头**:
```
Authorization: Bearer <token>
```

**成功响应** (200):
```json
{
  "code": 200,
  "message": "Success",
  "data": {
    "id": 1,
    "username": "john_doe",
    "email": "john@example.com",
    "phone": "+1234567890",
    "age": 28,
    "gender": "Male",
    "idNumber": "1234567890",
    "avatar": "https://api.example.com/avatars/user1.jpg",
    "userLevel": "Gold",
    "createdAt": "2024-01-15T10:30:00Z",
    "updatedAt": "2024-01-20T15:45:00Z"
  }
}
```

---

### 5. 更新用户信息

**端点**: `PUT /users/profile`

**描述**: 更新用户个人信息

**请求头**:
```
Content-Type: application/json
Authorization: Bearer <token>
```

**请求体** (所有字段可选):
```json
{
  "phone": "+1987654321",
  "age": 29,
  "gender": "Female",
  "idNumber": "0987654321"
}
```

**成功响应** (200):
```json
{
  "code": 200,
  "message": "Profile updated successfully",
  "data": {
    "id": 1,
    "username": "john_doe",
    "email": "john@example.com",
    "phone": "+1987654321",
    "age": 29,
    "gender": "Female",
    "idNumber": "0987654321",
    "updatedAt": "2024-01-20T16:00:00Z"
  }
}
```

---

## 航班 API

### 1. 搜索航班

**端点**: `POST /flights/search`

**描述**: 搜索可用航班

**请求头**:
```
Content-Type: application/json
```

**请求体**:
```json
{
  "departure": "New York",
  "destination": "Los Angeles",
  "departureDate": "2024-02-15",
  "returnDate": "2024-02-22",
  "passengers": 2,
  "tripType": "round-trip",
  "seatClass": "economy"
}
```

**请求字段说明**:

| 字段 | 类型 | 必需 | 说明 |
|------|------|------|------|
| departure | String | ✓ | 出发城市或机场代码 |
| destination | String | ✓ | 目的地城市或机场代码 |
| departureDate | String | ✓ | 出发日期 (YYYY-MM-DD) |
| returnDate | String | ✗ | 返回日期, 单程时不需要 |
| passengers | Integer | ✓ | 乘客数量 (1-9) |
| tripType | String | ✓ | 行程类型: one-way/round-trip |
| seatClass | String | ✗ | 舱等: economy/business/first, 默认economy |

**成功响应** (200):
```json
{
  "code": 200,
  "message": "Search successful",
  "data": [
    {
      "id": 101,
      "airline": "Sky Airways",
      "flightNumber": "SA123",
      "aircraft": "Boeing 777",
      "departure": "New York (JFK)",
      "destination": "Los Angeles (LAX)",
      "departureTime": "2024-02-15T08:00:00Z",
      "arrivalTime": "2024-02-15T11:30:00Z",
      "duration": "5h 30m",
      "distance": 2475,
      "stops": 0,
      "availableSeats": {
        "economy": 120,
        "business": 30,
        "first": 8
      },
      "pricing": {
        "economy": 249.99,
        "business": 789.99,
        "first": 1299.99
      },
      "amenities": ["WiFi", "Meal Service", "Entertainment"],
      "rating": 4.5,
      "reviews": 1250
    },
    {
      "id": 102,
      "airline": "Global Airlines",
      "flightNumber": "GA456",
      "aircraft": "Airbus A380",
      "departure": "New York (JFK)",
      "destination": "Los Angeles (LAX)",
      "departureTime": "2024-02-15T14:00:00Z",
      "arrivalTime": "2024-02-15T17:45:00Z",
      "duration": "5h 45m",
      "distance": 2475,
      "stops": 0,
      "availableSeats": {
        "economy": 280,
        "business": 60,
        "first": 12
      },
      "pricing": {
        "economy": 199.99,
        "business": 699.99,
        "first": 1199.99
      },
      "amenities": ["WiFi", "Meal Service", "Entertainment", "Lounge Access"],
      "rating": 4.7,
      "reviews": 2100
    }
  ]
}
```

---

### 2. 获取特色航班

**端点**: `GET /flights/featured`

**描述**: 获取推荐的特色航班

**请求头**:
```
Content-Type: application/json
```

**成功响应** (200):
```json
{
  "code": 200,
  "message": "Success",
  "data": [
    {
      "id": 201,
      "airline": "Premium Airways",
      "flightNumber": "PA789",
      "departure": "New York",
      "destination": "London",
      "departureTime": "2024-02-16T10:00:00Z",
      "arrivalTime": "2024-02-16T22:00:00Z",
      "price": 459.99,
      "discount": 20,
      "rating": 4.8,
      "badge": "Best Deal"
    }
  ]
}
```

---

### 3. 获取航班详情

**端点**: `GET /flights/{flightId}`

**描述**: 获取特定航班的详细信息

**请求头**:
```
Content-Type: application/json
```

**路径参数**:
| 参数 | 类型 | 说明 |
|------|------|------|
| flightId | Integer | 航班ID |

**成功响应** (200):
```json
{
  "code": 200,
  "message": "Success",
  "data": {
    "id": 101,
    "airline": "Sky Airways",
    "flightNumber": "SA123",
    "aircraft": "Boeing 777",
    "departure": "New York (JFK)",
    "destination": "Los Angeles (LAX)",
    "departureTime": "2024-02-15T08:00:00Z",
    "arrivalTime": "2024-02-15T11:30:00Z",
    "duration": "5h 30m",
    "distance": 2475,
    "stops": 0,
    "gate": "A12",
    "terminal": 1,
    "availableSeats": {
      "economy": 120,
      "business": 30,
      "first": 8
    },
    "pricing": {
      "economy": 249.99,
      "business": 789.99,
      "first": 1299.99
    },
    "amenities": ["WiFi", "Meal Service", "Entertainment"],
    "seatMap": "...",
    "baggage": {
      "checked": 2,
      "carry": 1
    },
    "cancellationPolicy": "Free cancellation up to 24 hours before departure"
  }
}
```

---

## 预订 API

### 1. 创建预订

**端点**: `POST /bookings/create`

**描述**: 创建新的航班预订

**请求头**:
```
Content-Type: application/json
Authorization: Bearer <token>
```

**请求体**:
```json
{
  "flightId": 101,
  "passengers": [
    {
      "title": "Mr",
      "firstName": "John",
      "lastName": "Doe",
      "email": "john@example.com",
      "dateOfBirth": "1996-05-15",
      "gender": "Male",
      "passportNumber": "AB123456",
      "passportExpiry": "2025-12-31",
      "nationality": "American"
    }
  ],
  "seatClass": "economy",
  "seats": ["12A", "12B"],
  "mealPreferences": ["Vegetarian", "Standard"],
  "specialRequests": "Window seat preferred"
}
```

**请求字段说明**:

| 字段 | 类型 | 必需 | 说明 |
|------|------|------|------|
| flightId | Integer | ✓ | 航班ID |
| passengers | Array | ✓ | 乘客信息数组 |
| seatClass | String | ✓ | 舱等 |
| seats | Array | ✓ | 选中的座位号数组 |
| mealPreferences | Array | ✗ | 用餐偏好 |
| specialRequests | String | ✗ | 特殊要求 |

**成功响应** (201):
```json
{
  "code": 201,
  "message": "Booking created successfully",
  "data": {
    "bookingId": "BK202402150001",
    "bookingReference": "SA7HK2",
    "status": "pending",
    "flightId": 101,
    "airline": "Sky Airways",
    "flightNumber": "SA123",
    "departure": "New York (JFK)",
    "destination": "Los Angeles (LAX)",
    "departureTime": "2024-02-15T08:00:00Z",
    "arrivalTime": "2024-02-15T11:30:00Z",
    "passengers": 1,
    "totalPrice": 249.99,
    "breakdown": {
      "baseFare": 199.99,
      "taxes": 30.00,
      "fees": 20.00
    },
    "expiresAt": "2024-02-14T12:00:00Z"
  }
}
```

---

### 2. 获取预订详情

**端点**: `GET /bookings/{bookingId}`

**描述**: 获取预订的详细信息

**请求头**:
```
Authorization: Bearer <token>
```

**路径参数**:
| 参数 | 类型 | 说明 |
|------|------|------|
| bookingId | String | 预订ID |

**成功响应** (200):
```json
{
  "code": 200,
  "message": "Success",
  "data": {
    "bookingId": "BK202402150001",
    "bookingReference": "SA7HK2",
    "status": "confirmed",
    "createdAt": "2024-02-14T10:30:00Z",
    "flightDetails": {
      "id": 101,
      "airline": "Sky Airways",
      "flightNumber": "SA123",
      "aircraft": "Boeing 777",
      "departure": "New York (JFK)",
      "destination": "Los Angeles (LAX)",
      "departureTime": "2024-02-15T08:00:00Z",
      "arrivalTime": "2024-02-15T11:30:00Z"
    },
    "passengers": [
      {
        "id": 1,
        "title": "Mr",
        "firstName": "John",
        "lastName": "Doe",
        "seat": "12A",
        "checkIn": true
      }
    ],
    "totalPrice": 249.99,
    "paymentStatus": "completed",
    "itinerary": "...",
    "boardingPass": "..."
  }
}
```

---

### 3. 修改预订

**端点**: `PUT /bookings/{bookingId}`

**描述**: 修改现有预订 (座位、乘客信息等)

**请求头**:
```
Content-Type: application/json
Authorization: Bearer <token>
```

**请求体**:
```json
{
  "seats": ["14A", "14B"],
  "mealPreferences": ["Vegan", "Standard"],
  "specialRequests": "Aisle seat needed"
}
```

**成功响应** (200):
```json
{
  "code": 200,
  "message": "Booking updated successfully",
  "data": {
    "bookingId": "BK202402150001",
    "seats": ["14A", "14B"],
    "mealPreferences": ["Vegan", "Standard"],
    "specialRequests": "Aisle seat needed",
    "updatedAt": "2024-02-14T11:00:00Z"
  }
}
```

---

### 4. 取消预订

**端点**: `DELETE /bookings/{bookingId}`

**描述**: 取消预订

**请求头**:
```
Authorization: Bearer <token>
```

**请求体**:
```json
{
  "reason": "Schedule conflict",
  "contactEmail": "john@example.com"
}
```

**成功响应** (200):
```json
{
  "code": 200,
  "message": "Booking cancelled successfully",
  "data": {
    "bookingId": "BK202402150001",
    "status": "cancelled",
    "cancellationTime": "2024-02-14T11:15:00Z",
    "refundAmount": 200.00,
    "refundStatus": "processing"
  }
}
```

---

## 订单 API

### 1. 获取用户订单列表

**端点**: `GET /orders`

**描述**: 获取当前用户的所有订单

**请求头**:
```
Authorization: Bearer <token>
```

**查询参数**:
| 参数 | 类型 | 说明 |
|------|------|------|
| status | String | 筛选状态: pending/confirmed/completed/cancelled |
| page | Integer | 页码, 默认1 |
| limit | Integer | 每页数量, 默认10 |
| sortBy | String | 排序字段: createdAt/departureTime |
| order | String | 排序方向: asc/desc |

**成功响应** (200):
```json
{
  "code": 200,
  "message": "Success",
  "data": {
    "orders": [
      {
        "orderId": "ORD202402150001",
        "bookingReference": "SA7HK2",
        "status": "confirmed",
        "createdAt": "2024-02-14T10:30:00Z",
        "flightDetails": {
          "airline": "Sky Airways",
          "flightNumber": "SA123",
          "departure": "New York",
          "destination": "Los Angeles",
          "departureTime": "2024-02-15T08:00:00Z"
        },
        "totalPrice": 249.99,
        "passengers": 1
      }
    ],
    "pagination": {
      "currentPage": 1,
      "totalPages": 5,
      "totalItems": 48,
      "itemsPerPage": 10
    }
  }
}
```

---

### 2. 获取订单详情

**端点**: `GET /orders/{orderId}`

**描述**: 获取单个订单的详细信息

**请求头**:
```
Authorization: Bearer <token>
```

**路径参数**:
| 参数 | 类型 | 说明 |
|------|------|------|
| orderId | String | 订单ID |

**成功响应** (200):
```json
{
  "code": 200,
  "message": "Success",
  "data": {
    "orderId": "ORD202402150001",
    "bookingReference": "SA7HK2",
    "status": "confirmed",
    "createdAt": "2024-02-14T10:30:00Z",
    "paidAt": "2024-02-14T10:35:00Z",
    "flightDetails": {
      "id": 101,
      "airline": "Sky Airways",
      "flightNumber": "SA123",
      "aircraft": "Boeing 777",
      "departure": "New York (JFK)",
      "destination": "Los Angeles (LAX)",
      "departureTime": "2024-02-15T08:00:00Z",
      "arrivalTime": "2024-02-15T11:30:00Z",
      "gate": "A12",
      "terminal": 1
    },
    "passengers": [
      {
        "id": 1,
        "firstName": "John",
        "lastName": "Doe",
        "seat": "12A",
        "mealPreference": "Vegetarian"
      }
    ],
    "pricing": {
      "baseFare": 199.99,
      "taxes": 30.00,
      "fees": 20.00,
      "totalPrice": 249.99
    },
    "paymentMethod": "Credit Card",
    "invoiceUrl": "https://api.example.com/invoices/INV202402150001.pdf",
    "itineraryUrl": "https://api.example.com/itineraries/ITI202402150001.pdf"
  }
}
```

---

## 支付 API

### 1. 发起支付

**端点**: `POST /payments/initiate`

**描述**: 发起支付流程

**请求头**:
```
Content-Type: application/json
Authorization: Bearer <token>
```

**请求体**:
```json
{
  "bookingId": "BK202402150001",
  "amount": 249.99,
  "currency": "USD",
  "paymentMethod": "credit_card",
  "cardDetails": {
    "cardholderName": "John Doe",
    "cardNumber": "4532XXXXXXXX1234",
    "expiryDate": "12/26",
    "cvv": "123"
  },
  "billingAddress": {
    "street": "123 Main St",
    "city": "New York",
    "state": "NY",
    "zipCode": "10001",
    "country": "USA"
  }
}
```

**成功响应** (200):
```json
{
  "code": 200,
  "message": "Payment initiated successfully",
  "data": {
    "transactionId": "TXN202402141001",
    "status": "processing",
    "amount": 249.99,
    "currency": "USD",
    "timestamp": "2024-02-14T10:30:00Z"
  }
}
```

---

### 2. 验证支付

**端点**: `POST /payments/verify`

**描述**: 验证支付结果

**请求头**:
```
Content-Type: application/json
Authorization: Bearer <token>
```

**请求体**:
```json
{
  "transactionId": "TXN202402141001",
  "verificationCode": "123456"
}
```

**成功响应** (200):
```json
{
  "code": 200,
  "message": "Payment verified successfully",
  "data": {
    "transactionId": "TXN202402141001",
    "status": "completed",
    "bookingStatus": "confirmed",
    "orderId": "ORD202402150001"
  }
}
```

---

## 通知 API

### 1. 订阅通知

**端点**: `POST /notifications/subscribe`

**描述**: 订阅特定类型的通知

**请求头**:
```
Content-Type: application/json
Authorization: Bearer <token>
```

**请求体**:
```json
{
  "types": ["flight_reminder", "price_drop", "booking_updates"],
  "channels": ["email", "sms"],
  "preferences": {
    "email": "john@example.com",
    "phone": "+1234567890"
  }
}
```

**成功响应** (201):
```json
{
  "code": 201,
  "message": "Subscription created successfully",
  "data": {
    "subscriptionId": "SUB202402140001",
    "types": ["flight_reminder", "price_drop", "booking_updates"],
    "channels": ["email", "sms"]
  }
}
```

---

### 2. 获取通知

**端点**: `GET /notifications`

**描述**: 获取用户通知列表

**请求头**:
```
Authorization: Bearer <token>
```

**查询参数**:
| 参数 | 类型 | 说明 |
|------|------|------|
| unreadOnly | Boolean | 仅返回未读通知 |
| type | String | 筛选通知类型 |
| limit | Integer | 返回数量, 默认20 |

**成功响应** (200):
```json
{
  "code": 200,
  "message": "Success",
  "data": [
    {
      "id": 1,
      "type": "flight_reminder",
      "title": "Flight Reminder",
      "message": "Your flight SA123 departs in 24 hours",
      "read": false,
      "createdAt": "2024-02-14T10:30:00Z"
    }
  ]
}
```

---

## 聊天 API

### 1. 获取聊天历史

**端点**: `GET /chat/history`

**描述**: 获取与客服的聊天历史

**请求头**:
```
Authorization: Bearer <token>
```

**查询参数**:
| 参数 | 类型 | 说明 |
|------|------|------|
| limit | Integer | 返回数量, 默认50 |
| offset | Integer | 偏移量, 默认0 |

**成功响应** (200):
```json
{
  "code": 200,
  "message": "Success",
  "data": [
    {
      "id": 1,
      "sender": "user",
      "message": "I have a question about my booking",
      "timestamp": "2024-02-14T10:00:00Z"
    },
    {
      "id": 2,
      "sender": "support",
      "message": "Hello! How can I help you today?",
      "timestamp": "2024-02-14T10:01:00Z"
    }
  ]
}
```

---

### 2. 发送消息

**端点**: `POST /chat/send`

**描述**: 发送聊天消息

**请求头**:
```
Content-Type: application/json
Authorization: Bearer <token>
```

**请求体**:
```json
{
  "message": "I would like to change my seat",
  "attachments": []
}
```

**成功响应** (201):
```json
{
  "code": 201,
  "message": "Message sent successfully",
  "data": {
    "id": 3,
    "sender": "user",
    "message": "I would like to change my seat",
    "timestamp": "2024-02-14T10:02:00Z"
  }
}
```

---

## 错误处理

### 常见错误响应

**400 - 请求参数错误**:
```json
{
  "code": 400,
  "message": "Invalid request parameters",
  "data": {
    "errors": {
      "email": "Invalid email format",
      "password": "Password must be at least 8 characters"
    }
  }
}
```

**401 - 未授权**:
```json
{
  "code": 401,
  "message": "Token expired or invalid",
  "data": null
}
```

**500 - 服务器错误**:
```json
{
  "code": 500,
  "message": "Internal server error",
  "data": null
}
```

---

## 生产部署检查清单

- [ ] API基础URL已配置为生产服务器地址
- [ ] 启用HTTPS/SSL加密
- [ ] 配置CORS跨域策略
- [ ] 实现请求限流和防DDoS
- [ ] 启用API请求日志记录
- [ ] 配置监控和告警
- [ ] 部署到负载均衡器
- [ ] 配置数据库备份
- [ ] 实施安全的密钥管理
- [ ] 进行安全渗透测试

---

## 版本历史

| 版本 | 日期 | 变更 |
|------|------|------|
| 1.0 | 2024-02-14 | 初版本发布 |
| 1.1 | 计划中 | 新增实时通知WebSocket支持 |
| 1.2 | 计划中 | 新增积分系统API |

---

## 联系支持

- **文档**: https://docs.skywings.example.com
- **支持邮箱**: support@skywings.example.com
- **问题反馈**: https://github.com/skywings/issues
