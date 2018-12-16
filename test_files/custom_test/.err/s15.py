outputStart = "Start of tinyOutput\n"
eol = "\n"
space = " "
def max2(a, b):
    result = 0
    if (a > b):
        return a
    else:
        result = b
    
    return result

def max3(a, b, c):
    if (a > b):
        if(a > c):
            return a
        else:
            return c
    else:
        if(b < c):
            return c
        return b


def calc1(a, b, c, d, e):
    x = c + max3(a, d, e) * 2.3 - max2(b, c) / max2(a, b) + 1.2
    y = x * a / 0.68 + c - max3(d, 1.25, max2(x, 1.5)) * e
    z = max2(x, y) * 1.4 / 0.5 + c - y
    print(x, space, y, space, z,  eol)
    result = max3(x+a, y-b*e, z)
    print(result, eol)
    return max3(x+a, y-b*e, z)

def calc2(f, g, h, i, j):
    x = 0
    if (f == g):
            x = g + h * 2.3 - max2(f, i) / j 
    else:
        if (f > g):
            x = j * max3(h, i*2, j+1.2-0.4)
        else:
            x = f+g-h*i/0.2+j
            
    print(x, eol)
    
    if(h <= i):
        x = x - max2(i, j*1.1-0.1) * f / g + 1.2 * h
    else:
        x = x + f * g / h + max2(i*j-1, i+j)
    print(x, eol)
    
    if(j >= i):
        x = x * max2(i+j, max2(f-g, h+i))
    else:
        x = x / f + j - 1.3 * h
    print(x, eol)
    
    if(g != h):
        x = f + 0.5 * x - i
    else:
        if(g < h):
            x = i /j + 0.98 * h - 0.1
        else:
            x = 1.0 + max2(x+1.1*j/h, x-0.1*f) * i + h
    print(x, eol)
    
    return x

def calc(g, i, h, j, c, a, d, f, b, e):
        return max2(calc1(g, i, h, j, c), calc2(a, d, f, b, e))

def main ():
    print(outputStart)
    result = 0.0
    a = float(input());
    b = float(input());
    c = float(input());
    d = float(input());
    e = float(input());
    f = float(input());
    g = float(input());
    h = float(input());
    i = float(input());
    j = float(input());
    result = calc(a, b, c, d, e, f, g, h, i, j)
    print(result, eol)
    return 0

if __name__ == "__main__":
    main()
