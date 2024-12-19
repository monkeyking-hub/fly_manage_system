

### 用户 API 文档

以下是针对你的用户管理 API 的文档，涵盖了各个接口的请求方式、路径、请求参数和返回值格式。

---

#### 1. **用户注册接口**

- **URL**: `/api/users/register`
- **方法**: `POST`
- **请求参数**（JSON 格式）:

  ```json
  {
      "username": "string",       // 用户名
      "password": "string",       // 密码
      "email": "string",          // 邮箱
      "phone": "string",          // 电话
      "age": 1,             // 年龄
      "gender": "string",         // 性别
      "idNumber": "string",       // 身份证号码
      "userLevel": 1        // 用户等级
  }
  ```

- **返回参数**（JSON 格式）:

  - **成功**:

    ```json
    {
        "code": 200,
        "message": "Registration successful",
        "data": {
            "id": 1,    // 用户ID
            "username": "string",
            "email": "string"
        }
    }
    ```

  - **失败**:

    ```json
    {
        "code": 500,
        "message": "Registration failed: Email already exists",
        "data": null
    }
    ```

---

#### 2. **用户登录接口**

- **URL**: `/api/users/login`
- **方法**: `POST`
- **请求参数**（JSON 格式）:

  ```json
  {
      "email": "string",         // 邮箱
      "password": "string"       // 密码
  }
  ```

- **返回参数**（JSON 格式）:

  - **成功**:

    ```json
    {
        "code": 200,
        "message": "Login successful",
        "data": {
            "id": 1,
            "username": "string",
            "email": "string",
            "phone": "string",
            "age": 1,
            "gender": "string",
            "idNumber": "string",
            "userLevel": 1
        }
    }
    ```

  - **失败**:

    ```json
    {
        "code": 500,
        "message": "Login failed: Invalid email or password",
        "data": null
    }
    ```

---

#### 3. **根据邮箱更改用户信息接口**

- **URL**: `/api/users/update`
- **方法**: `POST`
- **请求参数**（JSON 格式）:

  ```json
  {
      "id": 1,              // 用户ID
      "username": "string",       // 用户名 (可选)
      "email": "string",          // 邮箱 必选
      "phone": "string",          // 电话 (可选)
      "age": 1,             // 年龄 (可选)
      "gender": "F",         // 性别 (可选)
      "idNumber": "string",       // 身份证号码 (可选)
      "userLevel": 1        // 用户等级 (可选)
  }
  ```

- **返回参数**（JSON 格式）:

  - **成功**:

    ```json
    {
        "code": 200,
        "message": "Update successful",
        "data": {
            "id": 1
        }
    }
    ```

  - **失败**:

    ```json
    {
        "code": 500,
        "message": "Update failed: User not found",
        "data": null
    }
    ```

---

#### 4. **用户注销接口**

- **URL**: `/api/users/logout`
- **方法**: `POST`
- **请求参数**（JSON 格式）:

  ```json
  {
      "email": "string"          // 用户邮箱
  }
  ```

- **返回参数**（JSON 格式）:

  - **成功**:

    ```json
    {
        "code": 200,
        "message": "Logout successful",
        "data": null
    }
    ```

  - **失败**:

    ```json
    {
        "code": 500,
        "message": "Logout failed: User not found",
        "data": null
    }
    ```

---

#### 5. **根据 Email 获取用户信息接口**

- **URL**: `/api/users/get`
- **方法**: `POST`
- **请求参数**（JSON 格式）:

  ```json
  {
      "email": "string"          // 用户邮箱
  }
  ```

- **返回参数**（JSON 格式）:

  - **成功**:

    ```json
    {
        "code": 200,
        "message": "User retrieved successfully",
        "data": {
            "id": 1,
            "username": "string",
            "email": "string",
            "phone": "string",
            "age": 1,
            "gender": "string",
            "idNumber": "string",
            "userLevel": 1
        }
    }
    ```

  - **失败**:

    ```json
    {
        "code": 500,
        "message": "User not found",
        "data": null
    }
    ```

---



---

## API 文档：航班管理系统

明白了！为了避免误解，我会更新搜索航班信息接口的 `data` 字段，让它可以包含多个航班，表示查询多个目的地的航班。这样，用户就不会误以为只能查询一个目的地的航班。

以下是更新后的 API 文档：

---

## API 文档：搜索航班信息接口 (`/api/flights/search`)

### 描述：
通过出发地和多个目的地搜索航班，返回符合条件的航班列表。

### 请求方式：
`POST`

### 请求头：
- `Content-Type: application/json`

### 请求体：
```json
{
  "departure": "北京",
  "destinations": ["上海", "成都"]
}
```

### 请求参数说明：
| 参数           | 类型     | 必填 | 描述           |
|----------------|----------|------|----------------|
| departure      | string   | 是   | 出发地         |
| destinations   | array    | 是   | 目的地列表     |

### 响应体：
```json
{
  "code": 200,
  "message": "Search successful",
  "data": [
    {
      "id": 4787,
      "flightNumber": "3U5341",
      "departure": "北京",
      "destination": "上海",
      "departureTime": 1735971531,
      "arrivalTime": 1735974951,
      "firstClassSeats": 5,
      "economyClassSeats": 182,
      "firstClassPrice": 0,
      "economyClassPrice": 0,
      "airlineCompany": "四川航空",
      "aircraftModel": "JET",
      "boardingGate": "T2",
      "arrivalAirport": "T2",
      "departureAirport": "大兴国际机场",
      "hasMeal": 0,
      "isInternational": 0,
      "luggageSizeLimit": 20,
      "businessClassSeats": 18,
      "businessClassPrice": 0
    },
    {
      "id": 4788,
      "flightNumber": "3U5342",
      "departure": "北京",
      "destination": "成都",
      "departureTime": 1735971532,
      "arrivalTime": 1735974952,
      "firstClassSeats": 5,
      "economyClassSeats": 180,
      "firstClassPrice": 0,
      "economyClassPrice": 0,
      "airlineCompany": "四川航空",
      "aircraftModel": "JET",
      "boardingGate": "T3",
      "arrivalAirport": "T3",
      "departureAirport": "大兴国际机场",
      "hasMeal": 0,
      "isInternational": 0,
      "luggageSizeLimit": 20,
      "businessClassSeats": 20,
      "businessClassPrice": 0
    }
  ]
}
```

### 响应参数说明：
| 参数               | 类型      | 描述                               |
|--------------------|-----------|------------------------------------|
| code               | int       | 响应状态码                         |
| message            | string    | 响应信息                           |
| data               | array     | 符合条件的航班信息列表             |
| id                 | int       | 航班 ID                            |
| flightNumber       | string    | 航班号                             |
| departure          | string    | 出发地                             |
| destination        | string    | 目的地                             |
| departureTime      | int       | 出发时间（UNIX 时间戳）           |
| arrivalTime        | int       | 到达时间（UNIX 时间戳）           |
| firstClassSeats    | int       | 头等舱余量                         |
| economyClassSeats  | int       | 经济舱余量                         |
| firstClassPrice    | double    | 头等舱票价                         |
| economyClassPrice  | double    | 经济舱票价                         |
| airlineCompany     | string    | 航空公司                           |
| aircraftModel      | string    | 飞机型号                           |
| boardingGate       | string    | 登机口                             |
| arrivalAirport     | string    | 到达机场                           |
| departureAirport   | string    | 出发机场                           |
| hasMeal            | int       | 是否提供餐食 (0-否，1-是)         |
| isInternational    | int       | 是否为国际航班 (0-否，1-是)       |
| luggageSizeLimit   | int       | 行李大小限制 (kg)                 |
| businessClassSeats | int       | 商务舱座位数                       |
| businessClassPrice | double    | 商务舱票价                         |

### 错误响应：
```json
{
  "code": 500,
  "message": "Search failed: No flights found",
  "data": null
}
```

---

## API 文档：根据 ID 查询航班信息

### 描述
根据航班 ID 查询指定航班的详细信息。

---

### 请求方式
`POST`

---

### 请求 URL
```
/api/flights/queryById
```

---

### 请求头
- `Content-Type: application/json`

---

### 请求体
```json
{
  "id": 123
}
```

---

### 请求参数说明
| 参数名 | 类型 | 必填 | 描述         |
|--------|------|------|--------------|
| id     | int  | 是   | 航班的唯一 ID |

---

### 响应体
```json
{
  "code": 200,
  "message": "Query successful",
  "data": {
    "id": 4787,
    "flightNumber": "3U5341",
    "departure": "北京",
    "destination": "上海",
    "departureTime": 1735971531,
    "arrivalTime": 1735974951,
    "firstClassSeats": 5,
    "economyClassSeats": 182,
    "firstClassPrice": 0,
    "economyClassPrice": 0,
    "airlineCompany": "四川航空",
    "aircraftModel": "JET",
    "boardingGate": "T2",
    "arrivalAirport": "T2",
    "departureAirport": "大兴国际机场",
    "hasMeal": 0,
    "isInternational": 0,
    "luggageSizeLimit": 20,
    "businessClassSeats": 18,
    "businessClassPrice": 0
  }
}

```

---

### 错误响应示例
```json
{
  "code": 400,
  "message": "Query failed: Flight not found",
  "data": null
}
```


---

### 2. **创建航班信息接口** (`/api/flights/create`)

#### 描述：
用于创建新的航班信息，提供航班的详细信息。

#### 请求方式：
`POST`

#### 请求头：
- `Content-Type: application/json`

#### 请求体：
```json
{
  "flightNumber": "CA123",
  "departure": "北京",
  "destination": "上海",
  "departureTime": 1674532500,
  "arrivalTime": 1674540000,
  "firstClassSeats": 10,
  "economyClassSeats": 150,
  "firstClassPrice": 1500,
  "economyClassPrice": 500,
  "airlineCompany": "中国国际航空公司",
  "aircraftModel": "Boeing 737"
}
```

#### 请求参数说明：
| 参数              | 类型     | 必填 | 描述               |
|-------------------|----------|------|--------------------|
| flightNumber      | string   | 是   | 航班号             |
| departure         | string   | 是   | 出发地             |
| destination       | string   | 是   | 目的地             |
| departureTime     | int      | 是   | 出发时间（UNIX 时间戳） |
| arrivalTime       | int      | 是   | 到达时间（UNIX 时间戳） |
| firstClassSeats   | int      | 是   | 头等舱座位数       |
| economyClassSeats | int      | 是   | 经济舱座位数       |
| firstClassPrice   | int      | 是   | 头等舱票价         |
| economyClassPrice | int      | 是   | 经济舱票价         |
| airlineCompany    | string   | 是   | 航空公司           |
| aircraftModel     | string   | 是   | 飞机型号           |

#### 响应体：
```json
{
  "code": 200,
  "message": "Flight created successfully",
  "data": {
    "id": 1,
    "flightNumber": "CA123"
  }
}
```

#### 响应参数说明：
| 参数              | 类型      | 描述               |
|-------------------|-----------|--------------------|
| code              | int       | 响应状态码         |
| message           | string    | 响应信息           |
| data              | object    | 包含航班 ID 和 航班号的对象 |
| id                | int       | 新创建的航班 ID    |
| flightNumber      | string    | 航班号             |

#### 错误响应：
```json
{
  "code": 500,
  "message": "Flight creation failed: Duplicate flight number",
  "data": null
}
```

---

### 3. **修改航班信息接口** (`/api/flights/update`)

#### 描述：
用于修改航班的详细信息，支持修改任意字段。提供航班 ID 和需要更新的字段及其值。

#### 请求方式：
`POST`

#### 请求头：
- `Content-Type: application/json`

#### 请求体：
```json
{
  "id": 1,
  "departure": "上海虹桥",
  "destination": "广州白云",
  "departureTime": 1674633600,
  "arrivalTime": 1674648000,
  "flightNumber": "CA456",
  "firstClassSeats": 8,
  "businessClassSeats": 20,
  "economyClassSeats": 140,
  "firstClassPrice": 1800,
  "businessClassPrice": 1200,
  "economyClassPrice": 600,
  "departureAirport": "虹桥机场",
  "arrivalAirport": "白云机场",
  "boardingGate": "A12",
  "aircraftModel": "Airbus A320",
  "airlineCompany": "南方航空",
  "hasMeal": true,
  "luggageSizeLimit": 23,
  "isInternational": false
}
```

#### 请求参数说明：
| 参数              | 类型      | 必填 | 描述                                |
|-------------------|-----------|------|-------------------------------------|
| id                | int       | 是   | 航班 ID                            |
| departure         | string    | 否   | 出发地                              |
| destination       | string    | 否   | 目的地                              |
| departureTime     | int       | 否   | 出发时间（UNIX 时间戳）              |
| arrivalTime       | int       | 否   | 到达时间（UNIX 时间戳）              |
| flightNumber      | string    | 否   | 航班号                              |
| firstClassSeats   | int       | 否   | 头等舱座位数                        |
| businessClassSeats| int       | 否   | 商务舱座位数                        |
| economyClassSeats | int       | 否   | 经济舱座位数                        |
| firstClassPrice   | int       | 否   | 头等舱票价                          |
| businessClassPrice| int       | 否   | 商务舱票价                          |
| economyClassPrice | int       | 否   | 经济舱票价                          |
| departureAirport  | string    | 否   | 出发机场                            |
| arrivalAirport    | string    | 否   | 到达机场                            |
| boardingGate      | string    | 否   | 登机口                              |
| aircraftModel     | string    | 否   | 飞机型号                            |
| airlineCompany    | string    | 否   | 航空公司                            |
| hasMeal           | boolean   | 否   | 是否提供餐饮                        |
| luggageSizeLimit  | int       | 否   | 行李限制大小（kg）                   |
| isInternational   | boolean   | 否   | 是否为国际航班                      |

#### 响应体：
```json
{
  "code": 200,
  "message": "Flight updated successfully",
  "data": {
    "id": 1
  }
}
```

#### 响应参数说明：
| 参数      | 类型      | 描述              |
|-----------|-----------|-------------------|
| code      | int       | 响应状态码         |
| message   | string    | 响应信息           |
| data      | object    | 包含航班 ID 的对象 |
| id        | int       | 修改的航班 ID      |

#### 错误响应：
```json
{
  "code": 500,
  "message": "Flight update failed: Missing flight ID",
  "data": null
}
```
或
```json
{
  "code": 500,
  "message": "Flight update failed: Flight not found",
  "data": null
}
```

#### 错误描述：
| 错误码 | 错误信息                                        | 描述                                   |
|--------|------------------------------------------------|----------------------------------------|
| 500    | Flight update failed: Missing flight ID        | 请求中未提供必要的航班 ID              |
| 500    | Flight update failed: Flight not found         | 根据提供的航班 ID 未找到对应航班信息    |
| 500    | Flight update failed: Invalid JSON format      | 提供的 JSON 格式不正确                |
| 500    | Flight update failed: Database error           | 更新数据库时发生错误                   |

---

#### 示例：
##### 修改航班到达时间和票价：
**请求体**：
```json
{
  "id": 2,
  "arrivalTime": 1674672000,
  "economyClassPrice": 550
}
```

**响应体**：
```json
{
  "code": 200,
  "message": "Flight updated successfully",
  "data": {
    "id": 2
  }
}
```

---

### 4. **删除航班信息接口** (`/api/flights/delete`)

#### 描述：
根据航班 ID 删除指定的航班。

#### 请求方式：
`POST`

#### 请求头：
- `Content-Type: application/json`

#### 请求体：
```json
{
  "id": 1
}
```

#### 响应体：

```json
{
  "code": 200,
  "message": "Flight deleted successfully",
  "data": {
    "id": 1
  }
}
```


#### 错误响应：
```json
{
  "code": 500,
  "message": "Delete failed: Flight not found",
  "data": null
}
```

---


---

### **3. 订单管理接口**

#### **3.1 创建订单**
**描述**: 创建航班订单。  
**URL**: `/api/orders/create`  
**方法**: `POST`  
**请求体**:
```json
{
  "userId": 1,
  "flightId": 101,
  "seatType": "Economy",
  "price": 500.0
}
```
**响应**:  
成功:
```json
{
  "code": 200,
  "message": "Order created successfully",
  "data": {
    "id": 201,
    "status": "Pending payment"
  }
}
```
失败:
```json
{
  "code": 500,
  "message": "Order creation failed: Flight fully booked",
  "data": null
}
```

---

#### **3.2 查询订单**
**描述**: 根据用户 ID 查询订单信息。  
**URL**: `/api/orders/search`  
**方法**: `POST`  
**请求体**:
```json
{
  "userId": 1
}
```
**响应**:  
成功:
```json
{
  "code": 200,
  "message": "Query successful",
  "data": [
    {
      "id": 201,
      "flightNumber": "MU1234",
      "seatType": "Economy",
      "price": 500.0,
      "status": "Pending payment",
      "paymentTime": null
    }
  ]
}
```
失败:
```json
{
  "code": 500,
  "message": "Query failed: No orders found",
  "data": null
}
```

---

#### **3.3 更新订单状态**
**描述**: 更新订单状态（如支付、退款）。  
**URL**: `/api/orders/update`  
**方法**: `POST`  
**请求体**:
```json
{
  "orderId": 201,
  "status": "Paid",
  "paymentTime": 1712345678
}
```
**响应**:  
成功:
```json
{
  "code": 200,
  "message": "Order updated successfully",
  "data": {
    "orderId": 201,
    "status": "Paid"
  }
}
```
失败:
```json
{
  "code": 500,
  "message": "Order update failed: Invalid order ID",
  "data": null
}
```

---

### 统一说明
- **HTTP 状态码**: 固定返回 `200`。
- **请求参数**: 均通过请求体传递。
- **响应报文字段**:
  - `"code"`: 业务状态码，`200` 表示成功，`500` 表示失败。
  - `"message"`: 描述信息。
  - `"data"`: 成功时返回具体数据，失败时为 `null`。


明白了，如果不需要传递 `from` 和 `auth_code` 字段，且请求体采用 JSON 格式，下面是更新后的 API 文档。

---

### 1. 发送邮件接口 (`/api/send`)

#### 请求方法：
- **POST**

#### 请求 URL：
- `127.0.0.1:8080/api/send`

#### 请求头：
- `Content-Type: application/json`

#### 请求体（JSON 格式）：
- `to`: 收件人邮箱（**必填**）
- `subject`: 邮件主题（**必填**）
- `body`: 邮件正文（**必填**）

#### 请求示例：

```json
{
    "to": "123456@qq.com",
    "subject": "Test Email",
    "body": "This is a test email from C++ server." 
}
#### 响应体（JSON 格式）：
- `code`: 返回码，`200` 表示成功，`500` 表示失败。
- `message`: 响应消息，提供错误描述或成功信息。
- `data`: 返回数据，如果成功发送邮件，`data` 会是 `null`。

#### 响应示例：
- 成功：
```json
{
  "code": 200,
  "message": "Email sent successfully",
  "data": null
}
```

- 失败（如缺少必要字段）：
```json
{
  "code": 400,
  "message": "Missing required fields (to, subject, body)",
  "data": null
}
```

- 失败（其他错误）：
```json
{
  "code": 500,
  "message": "Error processing the request",
  "data": null
}
```

---

好的，下面是基于 Flask 实现的文件上传 API 的文档，文档内容详细说明了上传头像图片以及下载头像图片的接口。

---

### **头像上传与下载 API 文档**

#### **1. 上传头像图片**

- **接口描述**：
  用于接收客户端上传的用户头像图片。客户端需通过 `POST` 请求上传头像，并使用用户的 `email` 作为文件名保存头像。

- **请求 URL**：
  ```
  POST /upload-avatar
  ```

- **请求参数**：
  | 参数       | 类型   | 必须 | 说明                         |
  |------------|--------|------|------------------------------|
  | `email`    | string | 是   | 用户的邮箱，作为文件名保存   |
  | `avatar`   | file   | 是   | 用户上传的头像图片，支持 JPEG 格式 |

- **请求示例**：

  - **请求 Body**：使用 `multipart/form-data` 格式
  ```bash
  curl -X POST -F "email=user@example.com" -F "avatar=@profile_pic.jpg" http://127.0.0.1:5000/upload-avatar
  ```

- **响应格式**：
  - 成功上传：
    ```json
    {
      "message": "Avatar uploaded successfully",
      "file_path": "./resource/user@example.com.jpg"
    }
    ```

  - 错误响应：
    ```json
    {
      "error": "No file part"
    }
    ```

    ```json
    {
      "error": "No selected file"
    }
    ```

    ```json
    {
      "error": "Email or file missing"
    }
    ```

- **说明**：
  上传成功后，服务器会将头像保存为 `resource/<email>.jpg`，并返回成功消息和文件路径。如果出现错误，返回错误提示信息。

---

#### **2. 下载头像图片**

- **接口描述**：
  用于根据用户的 `email` 下载对应的头像图片。客户端可以通过指定 `email` 来获取用户头像。

- **请求 URL**：
  ```
  GET /download-avatar/<email>
  ```

- **请求参数**：
  | 参数   | 类型   | 必须 | 说明                       |
  |--------|--------|------|----------------------------|
  | `email`| string | 是   | 用户的邮箱，用于匹配头像图片 |

- **请求示例**：
  ```bash
  curl -O http://127.0.0.1:5000/download-avatar/user@example.com
  ```

- **响应格式**：
  - 成功响应：头像图片文件（如果图片存在）
  - 错误响应：
    ```json
    {
      "error": "Avatar not found"
    }
    ```

- **说明**：
  如果提供的 `email` 对应的头像文件存在，服务器将直接返回该文件。如果文件不存在，返回 404 错误并提供提示信息。

---

### **错误处理**

- **没有文件部分**：如果客户端没有上传文件，返回 400 错误，错误信息为 `"No file part"`。
- **没有选择文件**：如果上传的文件为空，返回 400 错误，错误信息为 `"No selected file"`。
- **缺少 `email` 或 `avatar`**：如果请求体中缺少 `email` 或 `avatar` 参数，返回 400 错误，错误信息为 `"Email or file missing"`。
- **文件未找到**：如果请求下载的头像文件不存在，返回 404 错误，错误信息为 `"Avatar not found"`。

---

### **API 使用示例**

#### **上传头像图片**

```bash
curl -X POST -F "email=user@example.com" -F "avatar=@profile_pic.jpg" http://127.0.0.1:5000/upload-avatar
```

响应：
```json
{
  "message": "Avatar uploaded successfully",
  "file_path": "./resource/user@example.com.jpg"
}
```

#### **下载头像图片**

```bash
curl -O http://127.0.0.1:5000/download-avatar/user@example.com
```

如果头像存在，则会下载头像图片。如果头像不存在，返回：

```json
{
  "error": "Avatar not found"
}
```

---

### **接口描述**

#### **1. 发送消息接口**
- **请求 URL**: `/api/chat/send`
- **请求方式**: `POST`
- **请求体**:
  ```json
  {
    "userId": 1,               // 用户ID（必填）
    "message": "Hello, World!", // 消息内容（必填）
    "timestamp": 1674532500     // 消息时间戳（必填）
  }
  ```

- **成功响应**:
  ```json
  {
    "code": 200,
    "message": "Message sent successfully",
    "data": {
      "id": 1,
      "userId": 1,
      "message": "Hello, World!",
      "timestamp": 1674532500
    }
  }
  ```

- **失败响应**:
  ```json
  {
    "code": 500,
    "message": "Message sending failed: Missing required fields",
    "data": null
  }
  ```

#### **2. 获取所有聊天记录接口**
- **请求 URL**: `/api/chat/history`
- **请求方式**: `GET`

- **成功响应**:
  ```json
  {
    "code": 200,
    "message": "Chat history retrieved successfully",
    "data": [
      {
        "id": 1,
        "userId": 1,
        "message": "Hello, World!",
        "timestamp": 1674532500
      },
      {
        "id": 2,
        "userId": 2,
        "message": "Hi there!",
        "timestamp": 1674532800
      }
    ]
  }
  ```

- **失败响应**:
  ```json
  {
    "code": 500,
    "message": "Failed to retrieve chat history",
    "data": null
  }
  ```




