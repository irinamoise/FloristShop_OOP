A C++ program designed to manage a florist shop, allowing users to add, update, and delete plants and flowers, create orders, and calculate revenue. The program uses object-oriented programming principles, STL containers, and exception handling to provide a robust and user-friendly experience.

## Features
Manage Inventory: Add, update, and delete plants and flowers.
Create Orders: Create orders for plants and flowers, with automatic stock management.
Revenue Calculation: Calculate revenue from plant and flower sales.
Exception Handling: Robust error handling for invalid inputs and edge cases.
STL Containers: Uses list, vector, map, and set for efficient data management.
Interactive Menus: User-friendly menus for easy navigation.


## Follow the on-screen menus to:
- Add, update, or delete plants and flowers.
- Create and view orders for plants and flowers.
- Calculate revenue from sales.
- View current stock and order details.

## Class Structure
The program is built using the following classes:

Articol: Base class for all items (plants and flowers).
Planta: Derived class for plants, with additional attributes like origin and maintenance.
Floare: Derived class for flowers, with additional attributes like lifespan and restocking days.
Comanda: Template class for managing orders.
AdminFlorarie: Singleton class for managing the entire florist shop (inventory, orders, and revenue).
