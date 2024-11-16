
# 航班票务管理系统 - HTTP 接口设计

## 用户模块接口

### 1. 用户注册接口

#### 请求
- **方法**: `POST`
- **URL**: `/api/users/register`
- **请求头**:  
  - `Content-Type: application/json`
- **请求体**:
  ```json
  {
    "username": "exampleUser",
    "password": "examplePassword",
    "email": "user@example.com",
    "phone": "12345678901",
    "age": 25,
    "gender": "male",
    "idCard": "123456789012345678"
  }
  ```

#### 响应
- **状态码**: `201 Created`（注册成功） 或 `400 Bad Request`（请求参数有误）
- **响应体**:
  ```json
  {
    "message": "User registered successfully",
    "userId": 123
  }
  ```

---

### 2. 用户登录接口

#### 请求
- **方法**: `POST`
- **URL**: `/api/users/login`
- **请求头**:  
  - `Content-Type: application/json`
- **请求体**:
  ```json
  {
    "username": "exampleUser",
    "password": "examplePassword"
  }
  ```

#### 响应
- **状态码**: `200 OK`（登录成功） 或 `401 Unauthorized`（用户名或密码错误）
- **响应体**（成功登录返回）:
  ```json
  {
    "message": "Login successful",
    "token": "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9..."
  }
  ```

---

### 3. 获取用户信息接口

#### 请求
- **方法**: `GET`
- **URL**: `/api/users/{userId}`
- **请求头**:  
  - `Authorization: Bearer <token>`
- **路径参数**: 
  - `{userId}`: 用户 ID

#### 响应
- **状态码**: `200 OK`（获取成功） 或 `404 Not Found`（用户不存在）
- **响应体**:
  ```json
  {
    "id": 123,
    "username": "exampleUser",
    "email": "user@example.com",
    "phone": "12345678901",
    "age": 25,
    "gender": "male",
    "idCard": "123456789012345678"
  }
  ```

---

### 4. 更新用户信息接口

#### 请求
- **方法**: `PUT`
- **URL**: `/api/users/{userId}`
- **请求头**:  
  - `Content-Type: application/json`
  - `Authorization: Bearer <token>`
- **路径参数**: 
  - `{userId}`: 用户 ID
- **请求体**:
  ```json
  {
    "email": "newemail@example.com",
    "phone": "98765432100",
    "age": 26
  }
  ```

#### 响应
- **状态码**: `200 OK`（更新成功） 或 `400 Bad Request`（参数错误） 或 `401 Unauthorized`（无权限）
- **响应体**:
  ```json
  {
    "message": "User updated successfully"
  }
  ```

---

### 5. 用户注销接口

#### 请求
- **方法**: `DELETE`
- **URL**: `/api/users/{userId}`
- **请求头**:  
  - `Authorization: Bearer <token>`
- **路径参数**: 
  - `{userId}`: 用户 ID

#### 响应
- **状态码**: `200 OK`（注销成功） 或 `401 Unauthorized`（无权限）
- **响应体**:
  ```json
  {
    "message": "User deleted successfully"
  }
  ```

---
