import os
import math

def sigmoid(x):
    return 1 / (1 + math.exp(-x))

def tanh(x):
    return math.tanh(x)

def exp(x):
    return math.exp(x)

def relu(x):
    return max(0, x)

def generate_function_data(func, range_start, range_end, num_points):
    # 计算步长
    step = (range_end - range_start) / (num_points - 1)
    x_values = []
    y_values = []
    x = range_start
    for _ in range(num_points):
        x_values.append(x)
        y_values.append(func(x))
        x += step
    return x_values, y_values

def save_to_header_file(filename, functions_data, num_points):
    # 获取当前脚本的目录
    current_dir = os.path.dirname(os.path.abspath(__file__))
    # 构造完整的路径
    file_path = os.path.join(current_dir, filename)
    
    with open(file_path, 'w') as f:
        f.write('#ifndef ACTIVATION_FUNCTIONS_H\n')
        f.write('#define ACTIVATION_FUNCTIONS_H\n\n')

        # 遍历每个函数的数据，生成多个数组
        for func_name, (x_values, y_values) in functions_data.items():
            f.write(f'const float {func_name}_values[{num_points}] = {{\n')
            for x, y in zip(x_values, y_values):
                f.write(f'    {y:.18f}, // x = {x:.18f}\n')
                # f.write(f'    {y:.18f},\n')
            f.write('};\n\n')

        f.write('#endif // ACTIVATION_FUNCTIONS_H\n')

    print(f"File saved to: {file_path}")

# 使用示例
range_start = 0   # 范围起始值
range_end = 5      # 范围结束值
num_points = 8192   # 采样点数

# 生成每个激活函数的数据
functions_data = {
    'sigmoid': generate_function_data(sigmoid, range_start, range_end, num_points),
    'tanh': generate_function_data(tanh, range_start, range_end, num_points),
    # 'relu': generate_function_data(relu, range_start, range_end, num_points),
    # 'exp': generate_function_data(exp, range_start, range_end, num_points),
}

# 保存到.h文件
save_to_header_file('table.h', functions_data, num_points)
