def find_longest_snake_sequence(grid):
        n = int(len(grid) ** 0.5)  # Assuming it's a square grid
        dp = [1] * n
        
        for i in range(1, n):
            for j in range(i):
                if (abs(grid[i] - grid[j]) == 1 or grid[i] - grid[j] == 0) == 1 and dp[i] < dp[j] + 1:
                    dp[i] = dp[j] + 1
                    
        max_length = max(dp)
        max_index = dp.index(max_length)
        
        sequence = []
        current_length = max_length
        
        for i in range(max_index, -1, -1):
            if dp[i] == current_length and (len(sequence) == 0 or abs(grid[i] - sequence[-1]) == 1):
                sequence.append(grid[i])
                current_length -= 1
                
        return list(reversed(sequence))

# Example usage
grid = [1, 2, 3, 4, 5, 4, 5, 8, 9]
print(grid)
result = find_longest_snake_sequence(grid)
print(result)
