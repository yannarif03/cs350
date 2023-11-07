import matplotlib.pyplot as plt
import pandas as pd
import numpy as np

# Generate file names based on the pattern
FILENAME = 'O1out.csv'
OPERATIONS = [
            'IMG_REGISTER',
            'IMG_ROT90CLKW',
            'IMG_BLUR',
            'IMG_SHARPEN',
            'IMG_VERTEDGES',
            'IMG_HORIZEDGES',
        ]

# Initialize the EWMAs and errors for each operation
ewma_estimators = {op: None for op in OPERATIONS}
misprediction_errors = {op: [] for op in OPERATIONS}
alpha = 0.7

# Add a new dictionary to hold the last observed sample for each operation
last_observed_sample = {op: None for op in OPERATIONS}

def update_ewma(ewma, new_sample, alpha):
    if ewma is None:
        return new_sample
    return alpha * new_sample + (1 - alpha) * ewma

def calculate_misprediction(ewma, actual):
    return abs(ewma - actual)

def process_data(df):
    for idx, row in df.iterrows():
        op = row['op']
        new_sample = row['completed'] - row['sent']
        
        # Update the EWMA for the operation
        prev_ewma = ewma_estimators[op]  # Store the previous EWMA for misprediction calculation
        ewma_estimators[op] = update_ewma(ewma_estimators[op], new_sample, alpha)
        
        # Calculate the misprediction error if there's a previous EWMA
        if prev_ewma is not None:
            error = calculate_misprediction(prev_ewma, new_sample)
            misprediction_errors[op].append(error)
            
def read_file(file_name, op):
    colnames=["id","sent","op","ow","clid","servid","recieved","start","completed"]    
    df = pd.read_csv(file_name,names=colnames,sep="[,:]",engine="python")
    
    # Filter rows based on opcode
    df = df[df['op'] == op]
    
    return df

# Process file
for op in OPERATIONS:
    df = read_file(FILENAME, op)
    process_data(df)
    
# After all files have been processed, calculate the average misprediction error for each operation
average_misprediction_errors = {op: np.mean(errors) for op, errors in misprediction_errors.items() if errors}
print("Average Misprediction Errors for Each Operation:")
for op in OPERATIONS:
    if op in average_misprediction_errors:
        error = average_misprediction_errors[op]
        print(f"{op}: {error}")
    else:
        print(f"{op}: No misprediction errors recorded")

# You might want to print out how good the predictions are
# You could compare the average misprediction error with the average response time for each operation
print("\nPrediction Quality:")
for op in OPERATIONS:
    if op in average_misprediction_errors and average_misprediction_errors[op] < ewma_estimators[op]:
        quality = "Good"
    else:
        quality = "Poor"
    print(f"{op}: {quality}")
    
# After processing all the data, print out the estimated lengths for the next operation
print("\nEstimated lengths for the next operation of each type:")
for op, ewma in ewma_estimators.items():
    print(f"{op}: {ewma}")
        
