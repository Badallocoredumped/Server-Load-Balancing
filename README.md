# 🧬 Server Load Balancing Using Genetic Algorithm

This project is focused on solving the **server load balancing** problem using an **evolutionary algorithm**, specifically a **Genetic Algorithm (GA)**.

In distributed systems, load balancing is essential to ensure that incoming tasks are efficiently distributed among multiple servers. The goal is to avoid overloading any single server while maximizing resource utilization and minimizing response time.

Traditional load balancing algorithms may not always adapt well to dynamic or complex environments. In this project, we use a Genetic Algorithm to find an optimized task-to-server assignment. Each solution (or "chromosome") represents a potential assignment of tasks to servers. Over several generations, the GA evolves better solutions by using selection, crossover, and mutation operations based on a fitness function that measures how balanced and efficient the load distribution is.

The key components of this project include:

- A simulation environment with tasks and servers
- Genetic Algorithm implementation for optimization
- Fitness function that penalizes server overloads and load imbalance
- Configurable parameters such as population size, mutation rate, and number of generations

This approach allows us to dynamically and intelligently allocate workloads, making it a scalable and adaptive solution for server load balancing.

