import yaml
import json
import subprocess
import csv
from pathlib import Path
import logging

logging.basicConfig(
    level=logging.INFO,
    format="%(asctime)s - %(levelname)s - %(message)s",
    datefmt="%Y-%m-%d %H:%M:%S"
)

# All files in the same directory
working_dir = Path(__file__).parent
config_path = working_dir / "config.yml"
list_path = working_dir / "scenarios.yml"
binary_path = working_dir / "maximal"
result_dir = working_dir / "results"

# Create results directory
result_dir.mkdir(exist_ok=True)

# Verify binary exists
if not binary_path.exists():
    logging.error(f"Binary not found: {binary_path}")
    exit(1)

# Read base config
with open(config_path, "r", encoding="utf-8") as f:
    base_config = yaml.safe_load(f)

input_path = base_config.get("input_path", "/home/ngnamhy/nckh/dataset/Dataset")
severity = base_config.get("severity", "debug")

# Read test scenarios
with open(list_path, "r", encoding="utf-8") as f:
    list_cfg = yaml.safe_load(f)

datasets = list_cfg["dataset"]
algorithms = list_cfg["algorithm"]

from datetime import datetime

# Run benchmarks
for dataset_name, dataset_info in datasets.items():
    current_datetime = datetime.now() 
    out_csv = result_dir / f"{dataset_name}_{current_datetime}.csv"
    
    # Create new CSV file
    if out_csv.exists(): 
        out_csv.unlink()
    
    with open(out_csv, "a", newline="", encoding="utf-8") as f_csv:
        writer = csv.writer(f_csv)
        writer.writerow(["algorithm", "dataset", "threshold", "check_count", "fwups_size", "estimated_algo_mem", "minwus", "runtime"])

        for threshold in dataset_info["threshold"]:
            for algo in algorithms:
                # Update config.yml
                config_data = {
                    "input_path": input_path,
                    "severity": severity,
                    "algorithm": algo,
                    "dataset": dataset_name,
                    "threshold": threshold,
                }
                
                with open(config_path, "w", encoding="utf-8") as out:
                    yaml.dump(config_data, out, default_flow_style=False)

                logging.info(f"Running: {dataset_name} | {algo} | {threshold}")

                # Run binary and capture JSON output
                try:
                    proc = subprocess.run(
                        [str(binary_path)], 
                        cwd=working_dir, 
                        capture_output=True, 
                        text=True,
                    )
                    
                    # Parse JSON from stdout
                    result = json.loads(proc.stdout.strip())
                    
                    # Extract fields
                    algo_out = result.get("algo", "")
                    dataset_out = result.get("dataset", "")
                    check_count = result.get("check_count", "")
                    fwups_size = result.get("fwups size", "")
                    estimated_algo_mem = result.get("estimated_algo_mem", "")
                    minwus = result.get("minwus", "")
                    runtime = result.get("runtime", "")
                    
                    # Write to CSV
                    writer.writerow([algo_out, dataset_out, threshold, check_count, fwups_size, estimated_algo_mem, minwus, runtime])
                    logging.info(f"Result: fwups={fwups_size}, runtime={runtime}ms, est_mem={estimated_algo_mem}KB")
                    
                except subprocess.TimeoutExpired:
                    logging.error(f"Timeout: {dataset_name} | {algo} | {threshold}")
                    writer.writerow([algo, dataset_name, threshold, "", "", "", "", "TIMEOUT"])
                except json.JSONDecodeError as e:
                    logging.error(f"JSON parse error: {dataset_name} | {algo} | {threshold}")
                    if proc.stdout:
                        logging.error(f"  stdout: {proc.stdout[:200]}")
                    if proc.stderr:
                        logging.error(f"  stderr: {proc.stderr[:200]}")
                    writer.writerow([algo, dataset_name, threshold, "", "", "", "", "ERROR"])
                except Exception as e:
                    logging.error(f"Error: {dataset_name} | {algo} | {threshold}: {e}")
                    writer.writerow([algo, dataset_name, threshold, "", "", "", "", "ERROR"])

logging.info(f"✓ All results saved to {result_dir}/")
