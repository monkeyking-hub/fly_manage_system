# Sky Wings 航班预订系统 - 现代化 API 文档

## 📋 概述

Sky Wings 是一个全新的现代化航班预订系统，采用先进的 Qt C++ 框架开发，提供无图片的高级用户界面设计。本文档详细说明了所有 API 端点的使用方法、数据格式和集成指南。

---

## 🌐 基础信息

### 服务地址
- **协议**: HTTP/HTTPS
- **基础URL**: `http://localhost:8080/api`
- **数据格式**: JSON
- **字符编码**: UTF-8
- **API版本**: v2.0

### 通用响应格式

所有API响应遵循统一的格式：

```json
{
  "code": 200,
  "message": "Success message",
  "data": {},
  "timestamp": "2024-01-15T10:30:00Z",
  "requestId": "req_1234567890"
}
```

| 字段 | 类型 | 说明 |
|------|------|------|
| code | Integer | 状态码 (200=成功, 4xx=客户端错误, 5xx=服务器错误) |
| message | String | 信息描述 |
| data | Object/Array | 返回的数据 |
| timestamp | String | 响应时间戳 (ISO 8601格式) |
| requestId | String | 唯一请求ID，用于追踪 |

### 状态码说明

| 状态码 | 含义 | 说明 |
|--------|------|------|
| 200 | OK | 请求成功 |
| 201 | Created | 资源创建成功 |
| 400 | Bad Request | 请求参数错误 |
| 401 | Unauthorized | 未授权/认证失败 |
| 403 | Forbidden | 禁止访问 |
| 404 | Not Found | 资源不存在 |
| 409 | Conflict | 资源冲突 |
| 422 | Unprocessable Entity | 请求格式正确但语义错误 |
| 429 | Too Many Requests | 请求频率超限 |
| 500 | Internal Server Error | 服务器内部错误 |
| 503 | Service Unavailable | 服务不可用 |

---

## 🔐 用户认证 API

### 1. 用户注册

**端点**: `POST /users/register`

**描述**: 创建新用户账户

**请求头**:
```
Content-Type: application/json
X-API-Version: 2.0
```

**请求体**:
```json
{
  "username": "john_doe",
  "email": "john@example.com",
  "password": "SecurePass123!",
  "confirmPassword": "SecurePass123!",
  "phone": "+1234567890",
  "age": 28,
  "gender": "Male",
  "idNumber": "123456789012345678",
  "firstName": "John",
  "lastName": "Doe"
}
```

**请求字段说明**:

| 字段 | 类型 | 必需 | 验证规则 | 说明 |
|------|------|------|----------|------|
| username | String | ✓ | 3-20字符, 字母数字下划线 | 用户名 |
| email | String | ✓ | 有效邮箱格式 | 邮箱地址，必须唯一 |
| password | String | ✓ | 8-50字符，包含大小写字母和数字 | 密码 |
| confirmPassword | String | ✓ | 必须与password相同 | 确认密码 |
| phone | String | ✓ | 国际电话号码格式 | 电话号码 |
| age | Integer | ✓ | 18-120 | 年龄 |
| gender | String | ✓ | Male/Female/Other | 性别 |
| idNumber | String | ✓ | 18位身份证号 | 身份证号码 |
| firstName | String | ✓ | 1-50字符 | 名字 |
| lastName | String | ✓ | 1-50字符 | 姓氏 |

**成功响应** (201):
```json
{
  "code": 201,
  "message": "Registration successful",
  "data": {
    "userId": 12345,
    "username": "john_doe",
    "email": "john@example.com",
    "phone": "+1234567890",
    "createdAt": "2024-01-15T10:30:00Z",
    "status": "active"
  },
  "timestamp": "2024-01-15T10:30:00Z",
  "requestId": "req_1234567890"
}
```

**失败响应** (400):
```json
{
  "code": 400,
  "message": "Validation failed",
  "data": {
    "errors": [
      {
        "field": "email",
        "message": "Email already exists"
      },
      {
        "field": "password",
        "message": "Password must contain at least 8 characters"
      }
    ]
  },
  "timestamp": "2024-01-15T10:30:00Z",
  "requestId": "req_1234567890"
}
```

### 2. 用户登录

**端点**: `POST /users/login`

**描述**: 用户身份验证

**请求体**:
```json
{
  "email": "john@example.com",
  "password": "SecurePass123!",
  "rememberMe": true,
  "deviceInfo": {
    "platform": "Windows",
    "browser": "Qt",
    "ipAddress": "192.168.1.100"
  }
}
```

**成功响应** (200):
```json
{
  "code": 200,
  "message": "Login successful",
  "data": {
    "token": "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9...",
    "refreshToken": "refresh_token_here",
    "expiresIn": 3600,
    "user": {
      "userId": 12345,
      "username": "john_doe",
      "email": "john@example.com",
      "firstName": "John",
      "lastName": "Doe",
      "phone": "+1234567890",
      "membershipLevel": "Gold",
      "profilePicture": "https://api.example.com/avatars/12345.jpg"
    }
  },
  "timestamp": "2024-01-15T10:30:00Z",
  "requestId": "req_1234567890"
}
```

### 3. 刷新令牌

**端点**: `POST /auth/refresh`

**描述**: 刷新访问令牌

**请求体**:
```json
{
  "refreshToken": "refresh_token_here"
}
```

### 4. 用户登出

**端点**: `POST /users/logout`

**描述**: 用户登出，使令牌失效

**请求头**:
```
Authorization: Bearer <access_token>
```

---

## ✈️ 航班搜索 API

### 1. 搜索航班

**端点**: `POST /flights/search`

**描述**: 根据条件搜索航班

**请求体**:
```json
{
  "tripType": "roundtrip",
  "from": "PEK",
  "to": "SHA",
  "departureDate": "2024-02-15",
  "returnDate": "2024-02-20",
  "passengers": {
    "adults": 2,
    "children": 1,
    "infants": 0
  },
  "class": "economy",
  "filters": {
    "directFlight": false,
    "refundable": true,
    "maxPrice": 5000,
    "preferredAirlines": ["CA", "MU", "CZ"],
    "departureTimeRange": "morning",
    "arrivalTimeRange": "afternoon"
  },
  "sort": {
    "field": "price",
    "order": "asc"
  },
  "pagination": {
    "page": 1,
    "limit": 20
  }
}
```

**请求字段说明**:

| 字段 | 类型 | 必需 | 说明 |
|------|------|------|------|
| tripType | String | ✓ | oneway/roundtrip/multicity |
| from | String | ✓ | 出发城市机场代码 |
| to | String | ✓ | 目的地城市机场代码 |
| departureDate | String | ✓ | 出发日期 (YYYY-MM-DD) |
| returnDate | String | | 返回日期 (往返行程必需) |
| passengers | Object | ✓ | 乘客信息 |
| passengers.adults | Integer | ✓ | 成人数量 (1-9) |
| passengers.children | Integer | | 儿童数量 (2-11岁) |
| passengers.infants | Integer | | 婴儿数量 (0-2岁) |
| class | String | ✓ | economy/business/first |
| filters | Object | | 过滤条件 |
| filters.directFlight | Boolean | | 仅直飞航班 |
| filters.refundable | Boolean | | 仅可退款航班 |
| filters.maxPrice | Integer | | 最高价格 (CNY) |
| filters.preferredAirlines | Array | | 优先航空公司代码 |
| filters.departureTimeRange | String | | morning/afternoon/evening/night |
| filters.arrivalTimeRange | String | | morning/afternoon/evening/night |
| sort | Object | | 排序条件 |
| sort.field | String | | price/duration/departure/arrival |
| sort.order | String | | asc/desc |
| pagination | Object | | 分页信息 |
| pagination.page | Integer | | 页码 (从1开始) |
| pagination.limit | Integer | | 每页数量 (1-50) |

**成功响应** (200):
```json
{
  "code": 200,
  "message": "Search completed",
  "data": {
    "flights": [
      {
        "flightId": "CA1234",
        "flightNumber": "CA1234",
        "airline": "Air China",
        "airlineCode": "CA",
        "aircraft": "Boeing 737-800",
        "from": {
          "airport": "PEK",
          "city": "Beijing",
          "terminal": "T3",
          "departure": "2024-02-15T08:30:00Z"
        },
        "to": {
          "airport": "SHA",
          "city": "Shanghai",
          "terminal": "T2",
          "arrival": "2024-02-15T10:45:00Z"
        },
        "duration": "2h 15m",
        "price": {
          "total": 1299,
          "base": 999,
          "tax": 200,
          "fees": 100,
          "currency": "CNY"
        },
        "availability": {
          "economy": 15,
          "business": 5,
          "first": 2
        },
        "features": [
          "wifi",
          "meal",
          "entertainment",
          "usb_charging"
        ],
        "direct": true,
        "refundable": true,
        "stops": []
      }
    ],
    "pagination": {
      "currentPage": 1,
      "totalPages": 5,
      "totalFlights": 98,
      "hasNext": true,
      "hasPrevious": false
    },
    "searchTime": 0.847,
    "filtersApplied": ["directFlight", "refundable"]
  },
  "timestamp": "2024-01-15T10:30:00Z",
  "requestId": "req_1234567890"
}
```

### 2. 获取航班详情

**端点**: `GET /flights/{flightId}`

**描述**: 获取特定航班的详细信息

**路径参数**:
- `flightId`: 航班ID

**成功响应** (200):
```json
{
  "code": 200,
  "message": "Flight details retrieved",
  "data": {
    "flightId": "CA1234",
    "flightNumber": "CA1234",
    "airline": "Air China",
    "airlineCode": "CA",
    "aircraft": "Boeing 737-800",
    "registration": "B-1234",
    "from": {
      "airport": "PEK",
      "city": "Beijing",
      "terminal": "T3",
      "gate": "E12",
      "departure": "2024-02-15T08:30:00Z",
      "timezone": "Asia/Shanghai"
    },
    "to": {
      "airport": "SHA",
      "city": "Shanghai",
      "terminal": "T2",
      "gate": "B24",
      "arrival": "2024-02-15T10:45:00Z",
      "timezone": "Asia/Shanghai"
    },
    "duration": "2h 15m",
    "distance": "1089 km",
    "price": {
      "economy": {
        "total": 1299,
        "base": 999,
        "tax": 200,
        "fees": 100
      },
      "business": {
        "total": 3299,
        "base": 2799,
        "tax": 300,
        "fees": 200
      },
      "first": {
        "total": 6999,
        "base": 6299,
        "tax": 400,
        "fees": 300
      },
      "currency": "CNY"
    },
    "availability": {
      "economy": 15,
      "business": 5,
      "first": 2
    },
    "features": [
      "wifi",
      "meal",
      "entertainment",
      "usb_charging",
      "power_outlet"
    ],
    "baggage": {
      "carryOn": {
        "weight": 7,
        "dimensions": "55x40x20 cm"
      },
      "checked": {
        "weight": 23,
        "dimensions": "158 cm (sum)"
      }
    },
    "direct": true,
    "refundable": true,
    "cancellationPolicy": {
      "fullRefundBefore": "2024-02-14T08:30:00Z",
      "partialRefundBefore": "2024-02-15T02:30:00Z",
      "noRefundAfter": "2024-02-15T02:30:00Z"
    },
    "status": "scheduled",
    "onTimePerformance": 92.5
  },
  "timestamp": "2024-01-15T10:30:00Z",
  "requestId": "req_1234567890"
}
```

### 3. 获取特色航班

**端点**: `GET /flights/featured`

**描述**: 获取推荐的热门航班

**查询参数**:
- `limit`: 返回数量限制 (默认: 10)
- `region`: 地区筛选 (asia/europe/americas)
- `type`: 类型筛选 (international/domestic)

**成功响应** (200):
```json
{
  "code": 200,
  "message": "Featured flights retrieved",
  "data": {
    "flights": [
      {
        "flightId": "CA1234",
        "flightNumber": "CA1234",
        "route": "Beijing → Shanghai",
        "price": 1299,
        "discount": 20,
        "originalPrice": 1624,
        "departure": "2024-02-15T08:30:00Z",
        "features": ["direct", "refundable", "meal"],
        "rating": 4.8,
        "reviews": 1247
      }
    ],
    "promotions": [
      {
        "code": "SPRING20",
        "discount": 20,
        "description": "Spring sale - 20% off",
        "validUntil": "2024-03-31T23:59:59Z"
      }
    ]
  },
  "timestamp": "2024-01-15T10:30:00Z",
  "requestId": "req_1234567890"
}
```

---

## 📅 预订管理 API

### 1. 创建预订

**端点**: `POST /bookings/create`

**描述**: 创建航班预订

**请求头**:
```
Authorization: Bearer <access_token>
Content-Type: application/json
```

**请求体**:
```json
{
  "flightId": "CA1234",
  "tripType": "oneway",
  "passengers": [
    {
      "type": "adult",
      "firstName": "John",
      "lastName": "Doe",
      "gender": "Male",
      "birthDate": "1990-05-15",
      "nationality": "CN",
      "idNumber": "123456789012345678",
      "idType": "passport",
      "idExpiry": "2025-05-15",
      "phone": "+1234567890",
      "email": "john@example.com"
    }
  ],
  "class": "economy",
  "services": {
    "extraBaggage": true,
    "mealPreference": "vegetarian",
    "seatPreference": "window",
    "travelInsurance": true
  },
  "payment": {
    "method": "credit_card",
    "currency": "CNY"
  }
}
```

**成功响应** (201):
```json
{
  "code": 201,
  "message": "Booking created successfully",
  "data": {
    "bookingId": "BK2024021500001",
    "bookingReference": "ABC123",
    "status": "pending_payment",
    "createdAt": "2024-02-15T10:30:00Z",
    "expiresAt": "2024-02-15T12:30:00Z",
    "totalAmount": 1299,
    "currency": "CNY",
    "flight": {
      "flightId": "CA1234",
      "flightNumber": "CA1234",
      "from": "PEK",
      "to": "SHA",
      "departure": "2024-02-15T08:30:00Z",
      "arrival": "2024-02-15T10:45:00Z"
    },
    "passengers": [
      {
        "passengerId": "P123456",
        "name": "John Doe",
        "seat": "12A",
        "ticketNumber": "1234567890123"
      }
    ],
    "paymentUrl": "https://payment.example.com/pay/BK2024021500001"
  },
  "timestamp": "2024-02-15T10:30:00Z",
  "requestId": "req_1234567890"
}
```

### 2. 获取预订列表

**端点**: `GET /bookings`

**描述**: 获取用户的所有预订

**请求头**:
```
Authorization: Bearer <access_token>
```

**查询参数**:
- `status`: 预订状态筛选 (pending/paid/cancelled/completed)
- `page`: 页码 (默认: 1)
- `limit`: 每页数量 (默认: 20)

**成功响应** (200):
```json
{
  "code": 200,
  "message": "Bookings retrieved",
  "data": {
    "bookings": [
      {
        "bookingId": "BK2024021500001",
        "bookingReference": "ABC123",
        "status": "paid",
        "createdAt": "2024-02-15T10:30:00Z",
        "totalAmount": 1299,
        "currency": "CNY",
        "flight": {
          "flightNumber": "CA1234",
          "route": "Beijing → Shanghai",
          "departure": "2024-02-15T08:30:00Z",
          "arrival": "2024-02-15T10:45:00Z"
        },
        "passengers": 1,
        "canCancel": true,
        "canModify": true
      }
    ],
    "pagination": {
      "currentPage": 1,
      "totalPages": 3,
      "totalBookings": 25
    }
  },
  "timestamp": "2024-02-15T10:30:00Z",
  "requestId": "req_1234567890"
}
```

### 3. 获取预订详情

**端点**: `GET /bookings/{bookingId}`

**描述**: 获取特定预订的详细信息

**成功响应** (200):
```json
{
  "code": 200,
  "message": "Booking details retrieved",
  "data": {
    "bookingId": "BK2024021500001",
    "bookingReference": "ABC123",
    "status": "paid",
    "createdAt": "2024-02-15T10:30:00Z",
    "updatedAt": "2024-02-15T11:00:00Z",
    "totalAmount": 1299,
    "currency": "CNY",
    "paymentStatus": "completed",
    "paymentMethod": "credit_card",
    "flight": {
      "flightId": "CA1234",
      "flightNumber": "CA1234",
      "airline": "Air China",
      "from": {
        "airport": "PEK",
        "city": "Beijing",
        "terminal": "T3",
        "departure": "2024-02-15T08:30:00Z"
      },
      "to": {
        "airport": "SHA",
        "city": "Shanghai",
        "terminal": "T2",
        "arrival": "2024-02-15T10:45:00Z"
      },
      "class": "economy",
      "seat": "12A"
    },
    "passengers": [
      {
        "passengerId": "P123456",
        "name": "John Doe",
        "type": "adult",
        "seat": "12A",
        "ticketNumber": "1234567890123",
        "baggage": {
          "carryOn": 1,
          "checked": 1
        }
      }
    ],
    "services": {
      "meal": "vegetarian",
      "insurance": true
    },
    "cancellationPolicy": {
      "canCancel": true,
      "refundAmount": 1299,
      "deadline": "2024-02-14T08:30:00Z"
    }
  },
  "timestamp": "2024-02-15T10:30:00Z",
  "requestId": "req_1234567890"
}
```

### 4. 取消预订

**端点**: `POST /bookings/{bookingId}/cancel`

**描述**: 取消预订

**请求体**:
```json
{
  "reason": "Change of plans",
  "refundMethod": "original"
}
```

---

## 💳 支付 API

### 1. 初始化支付

**端点**: `POST /payments/initiate`

**描述**: 初始化支付流程

**请求体**:
```json
{
  "bookingId": "BK2024021500001",
  "amount": 1299,
  "currency": "CNY",
  "method": "credit_card",
  "returnUrl": "https://app.example.com/payment/success",
  "cancelUrl": "https://app.example.com/payment/cancel"
}
```

**成功响应** (200):
```json
{
  "code": 200,
  "message": "Payment initiated",
  "data": {
    "paymentId": "PAY2024021500001",
    "paymentUrl": "https://payment.example.com/pay/PAY2024021500001",
    "qrCode": "data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAA...",
    "expiresAt": "2024-02-15T12:30:00Z",
    "methods": [
      "credit_card",
      "alipay",
      "wechat_pay",
      "bank_transfer"
    ]
  },
  "timestamp": "2024-02-15T10:30:00Z",
  "requestId": "req_1234567890"
}
```

### 2. 确认支付状态

**端点**: `GET /payments/{paymentId}/status`

**描述**: 查询支付状态

**成功响应** (200):
```json
{
  "code": 200,
  "message": "Payment status retrieved",
  "data": {
    "paymentId": "PAY2024021500001",
    "status": "completed",
    "amount": 1299,
    "currency": "CNY",
    "method": "credit_card",
    "paidAt": "2024-02-15T11:15:00Z",
    "transactionId": "TXN1234567890",
    "receiptUrl": "https://receipt.example.com/TXN1234567890"
  },
  "timestamp": "2024-02-15T10:30:00Z",
  "requestId": "req_1234567890"
}
```

---

## 👤 用户资料 API

### 1. 获取用户资料

**端点**: `GET /users/profile`

**描述**: 获取当前用户的详细资料

**请求头**:
```
Authorization: Bearer <access_token>
```

**成功响应** (200):
```json
{
  "code": 200,
  "message": "Profile retrieved",
  "data": {
    "userId": 12345,
    "username": "john_doe",
    "email": "john@example.com",
    "phone": "+1234567890",
    "profile": {
      "firstName": "John",
      "lastName": "Doe",
      "gender": "Male",
      "birthDate": "1990-05-15",
      "nationality": "CN",
      "avatar": "https://api.example.com/avatars/12345.jpg"
    },
    "preferences": {
      "language": "zh-CN",
      "currency": "CNY",
      "timezone": "Asia/Shanghai",
      "seatPreference": "window",
      "mealPreference": "vegetarian"
    },
    "membership": {
      "level": "Gold",
      "points": 15420,
      "tier": "Gold",
      "benefits": [
        "priority_checking",
        "lounge_access",
        "extra_baggage"
      ]
    },
    "statistics": {
      "totalFlights": 47,
      "totalDistance": 125430,
      "favoriteDestinations": ["Shanghai", "Beijing", "Guangzhou"],
      "preferredAirlines": ["CA", "MU", "CZ"]
    }
  },
  "timestamp": "2024-02-15T10:30:00Z",
  "requestId": "req_1234567890"
}
```

### 2. 更新用户资料

**端点**: `PUT /users/profile`

**描述**: 更新用户资料信息

**请求体**:
```json
{
  "firstName": "John",
  "lastName": "Smith",
  "phone": "+1234567890",
  "preferences": {
    "language": "en-US",
    "currency": "USD",
    "seatPreference": "aisle",
    "mealPreference": "regular"
  }
}
```

---

## 📊 统计和分析 API

### 1. 获取用户统计

**端点**: `GET /users/statistics`

**描述**: 获取用户的飞行统计数据

**成功响应** (200):
```json
{
  "code": 200,
  "message": "Statistics retrieved",
  "data": {
    "flights": {
      "total": 47,
      "thisYear": 12,
      "thisMonth": 2
    },
    "distance": {
      "total": 125430,
      "thisYear": 32150,
      "unit": "km"
    },
    "savings": {
      "total": 3250,
      "thisYear": 850,
      "currency": "CNY"
    },
    "destinations": {
      "visited": 23,
      "countries": 8,
      "favorites": [
        {
          "city": "Shanghai",
          "visits": 8,
          "airport": "SHA"
        },
        {
          "city": "Beijing",
          "visits": 6,
          "airport": "PEK"
        }
      ]
    },
    "airlines": {
      "mostUsed": "Air China",
      "flights": 15,
      "loyaltyPoints": 15420
    }
  },
  "timestamp": "2024-02-15T10:30:00Z",
  "requestId": "req_1234567890"
}
```

---

## 🔍 搜索和推荐 API

### 1. 获取热门目的地

**端点**: `GET /destinations/popular`

**描述**: 获取热门目的地推荐

**查询参数**:
- `limit`: 返回数量 (默认: 10)
- `region`: 地区筛选
- `season`: 季节筛选 (spring/summer/autumn/winter)

**成功响应** (200):
```json
{
  "code": 200,
  "message": "Popular destinations retrieved",
  "data": {
    "destinations": [
      {
        "city": "Shanghai",
        "country": "China",
        "airport": "SHA",
        "popularity": 95,
        "averagePrice": 899,
        "description": "Modern metropolis with stunning skyline",
        "highlights": ["The Bund", "Yu Garden", "Shanghai Tower"],
        "bestTimeToVisit": "March-May, September-November",
        "imageUrl": "https://images.example.com/shanghai.jpg"
      }
    ],
    "trends": {
      "rising": ["Chengdu", "Xiamen", "Qingdao"],
      "seasonal": ["Harbin (winter)", "Sanya (summer)"]
    }
  },
  "timestamp": "2024-02-15T10:30:00Z",
  "requestId": "req_1234567890"
}
```

---

## 📱 移动端专用 API

### 1. 推送通知注册

**端点**: `POST /notifications/register`

**描述**: 注册设备推送通知

**请求体**:
```json
{
  "deviceId": "device_unique_id",
  "platform": "android",
  "token": "push_notification_token",
  "preferences": {
    "bookingUpdates": true,
    "flightChanges": true,
    "promotions": false
  }
}
```

---

## 🚨 错误处理

### 错误响应格式

所有错误响应都遵循统一格式：

```json
{
  "code": 400,
  "message": "Validation failed",
  "error": "VALIDATION_ERROR",
  "details": {
    "field": "email",
    "reason": "Invalid email format"
  },
  "timestamp": "2024-02-15T10:30:00Z",
  "requestId": "req_1234567890"
}
```

### 常见错误代码

| 错误代码 | 说明 | 解决方案 |
|----------|------|----------|
| VALIDATION_ERROR | 请求参数验证失败 | 检查请求参数格式和必填字段 |
| AUTHENTICATION_FAILED | 身份验证失败 | 检查token是否有效 |
| AUTHORIZATION_DENIED | 权限不足 | 确认用户有相应权限 |
| RESOURCE_NOT_FOUND | 资源不存在 | 检查资源ID是否正确 |
| RATE_LIMIT_EXCEEDED | 请求频率超限 | 降低请求频率 |
| SERVER_ERROR | 服务器内部错误 | 稍后重试或联系技术支持 |

---

## 🔧 开发工具和 SDK

### SDK 支持

我们提供以下语言的 SDK：

- **C++/Qt**: `skywings-qt-sdk v2.0.0`
- **JavaScript**: `skywings-js-sdk v2.0.0`
- **Python**: `skywings-python-sdk v2.0.0`
- **Java**: `skywings-java-sdk v2.0.0`

### 示例代码

#### C++/Qt 示例

```cpp
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QJsonDocument>

// 搜索航班示例
void searchFlights() {
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    
    QJsonObject searchParams;
    searchParams["from"] = "PEK";
    searchParams["to"] = "SHA";
    searchParams["departureDate"] = "2024-02-15";
    searchParams["class"] = "economy";
    
    QJsonDocument doc(searchParams);
    QByteArray data = doc.toJson();
    
    QUrl url("http://localhost:8080/api/flights/search");
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("Authorization", "Bearer <access_token>");
    
    QNetworkReply *reply = manager->post(request, data);
    connect(reply, &QNetworkReply::finished, [reply]() {
        if (reply->error() == QNetworkReply::NoError) {
            QJsonDocument response = QJsonDocument::fromJson(reply->readAll());
            // 处理响应数据
        }
        reply->deleteLater();
    });
}
```

---

## 📋 更新日志

### v2.0.0 (2024-02-15)
- 🎉 全新现代化 API 架构
- ✨ 新增用户认证和授权系统
- ✨ 新增高级搜索和过滤功能
- ✨ 新增实时航班状态更新
- 🔒 增强安全性和数据保护
- 📱 优化移动端支持
- 🚀 提升 API 性能和响应速度

### v1.5.0 (2024-01-01)
- 🐛 修复已知问题
- ⚡ 性能优化
- 📚 完善文档

---

## 📞 技术支持

如需技术支持或有任何问题，请联系我们：

- **邮箱**: api-support@skywings.com
- **电话**: +86 400-123-4567
- **在线客服**: https://support.skywings.com
- **开发者社区**: https://community.skywings.com

---

## 📄 许可证

本 API 文档遵循 [MIT License](https://opensource.org/licenses/MIT)。

---

**© 2024 Sky Wings 航班预订系统. 保留所有权利。**