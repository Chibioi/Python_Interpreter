# A simple Python test file for an interpreter

# --- Section 1: Basic Operations ---
print("--- Section 1: Basic Operations ---")

# Variable assignment and arithmetic
x = 10
y = 5
z = x + y
print(f"The sum of x and y is: {z}")

# String concatenation
greeting = "Hello"
name = "World"
message = greeting + ", " + name + "!"
print(message)

# Boolean logic
is_true = True
is_false = False
print(f"Is true and false? {is_true and is_false}")
print(f"Is true or false? {is_true or is_false}")

# --- Section 2: Control Flow ---
print("\n--- Section 2: Control Flow ---")

# If-else statement
temperature = 25
if temperature > 30:
    print("It's a hot day.")
elif temperature > 20:
    print("It's a pleasant day.")
else:
    print("It's a bit chilly.")

# For loop
fruits = ["apple", "banana", "cherry"]
print("Here are some fruits:")
for fruit in fruits:
    print(fruit)

# While loop
count = 0
print("Counting to 3:")
while count < 3:
    print(count)
    count += 1

# --- Section 3: Functions ---
print("\n--- Section 3: Functions ---")

# A simple function with a return value


def multiply(a, b):
    return a * b


result = multiply(4, 6)
print(f"4 multiplied by 6 is: {result}")

# A function with no return value


def greet(person):
    print(f"Hello, {person}!")


greet("Alice")

# --- Section 4: Data Structures ---
print("\n--- Section 4: Data Structures ---")

# List
my_list = [1, 2, 3, "four"]
print(f"The list is: {my_list}")
print(f"The first element is: {my_list[0]}")

# Dictionary
my_dict = {"name": "Bob", "age": 30, "city": "New York"}
print(f"The dictionary is: {my_dict}")
print(f"Bob's age is: {my_dict['age']}")

# --- Section 5: Error Handling ---
print("\n--- Section 5: Error Handling ---")

# Try-except block for division by zero
try:
    result = 10 / 0
except ZeroDivisionError as e:
    print(f"Caught an error: {e}")

print("\n--- End of test file ---")
