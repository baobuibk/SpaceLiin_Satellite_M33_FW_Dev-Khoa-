#ifndef _ERROR_CODE_H_
#define _ERROR_CODE_H_

typedef enum _error_code_t_
{
    // Mã lỗi chung
    ERROR_OK,               // Thành công
    ERROR_FAIL,             // Lỗi chung
    ERROR_TIMEOUT,          // Hết thời gian chờ
    ERROR_INVALID_PARAM,    // Tham số không hợp lệ
    ERROR_NOT_READY,        // Thiết bị chưa sẵn sàng
    ERROR_BUSY,             // Thiết bị đang bận
    ERROR_NOT_SUPPORTED,    // Chức năng không được hỗ trợ
    ERROR_OUT_OF_MEMORY,    // Hết bộ nhớ
    
    // Mã lỗi cho BUFFER
    ERROR_BUFFER_EMPTY,
    ERROR_BUFFER_FULL,

    // Mã lỗi cho UART/USART
    ERROR_UART_OVERRUN,     // Lỗi overrun (dữ liệu bị ghi đè)
    ERROR_UART_NOT_INIT,    // UART chưa được khởi tạo
    ERROR_UART_FRAMING,     // Lỗi framing (dữ liệu không đúng định dạng)
    ERROR_UART_PARITY,      // Lỗi parity
    ERROR_UART_TIMEOUT,     // Timeout khi truyền/nhận
    ERROR_UART_NOISE,       // Lỗi nhiễu trên đường truyền

} error_code_t;

#endif /* _ERROR_CODE_H_ */


