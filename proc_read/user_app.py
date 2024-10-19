def main():
    
    with open('/proc/test_module', 'r') as driver_handler:
        msg_from_kernelspace = driver_handler.readline()
        print(msg_from_kernelspace)
    return

main()
