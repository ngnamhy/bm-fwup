import pandas as pd
import matplotlib.pyplot as plt
from pathlib import Path
import logging

logging.basicConfig(
    level=logging.INFO,
    format="%(asctime)s - %(levelname)s - %(message)s",
    datefmt="%Y-%m-%d %H:%M:%S"
)

# Setup directories
result_dir = Path("results")
out_dir = Path("plots")
out_dir.mkdir(exist_ok=True)

# Plot styling
markers = ["o", "s", "D", "^", "v", "x", "*", "P", "H"]
colors = ["#1f77b4", "#ff7f0e", "#2ca02c", "#d62728", "#9467bd", "#8c564b", "#e377c2", "#7f7f7f", "#bcbd22"]

# Process each dataset CSV
for csv_file in result_dir.glob("*.csv"):
    dataset_name = csv_file.stem
    logging.info(f"Processing {dataset_name}...")
    
    df = pd.read_csv(csv_file)
    
    # Convert to numeric
    df["threshold"] = pd.to_numeric(df["threshold"], errors="coerce")
    df["runtime"] = pd.to_numeric(df["runtime"], errors="coerce")
    df["estimated_algo_mem"] = pd.to_numeric(df["estimated_algo_mem"], errors="coerce")
    df["check_count"] = pd.to_numeric(df["check_count"], errors="coerce")
    df["fwups_size"] = pd.to_numeric(df["fwups_size"], errors="coerce")
    df["minwus"] = pd.to_numeric(df["minwus"], errors="coerce")
    
    # Sort by threshold for better line plots
    df = df.sort_values("threshold")
    
    # 1. Runtime vs Threshold
    plt.figure(figsize=(10, 6))
    for i, (algo, group) in enumerate(df.groupby("algorithm")):
        plt.plot(
            group["threshold"],
            group["runtime"],
            marker=markers[i % len(markers)],
            color=colors[i % len(colors)],
            label=algo,
            linewidth=2,
            markersize=8
        )
    plt.xlabel("Threshold (%)", fontsize=12)
    plt.ylabel("Runtime (ms)", fontsize=12)
    plt.title(f"Runtime vs Threshold - {dataset_name.upper()}", fontsize=14, fontweight="bold")
    plt.legend(fontsize=10)
    plt.grid(True, linestyle="--", alpha=0.6)
    plt.tight_layout()
    plt.savefig(out_dir / f"{dataset_name}_runtime.png", dpi=150, bbox_inches="tight")
    plt.close()
    
    # 2. Memory vs Threshold
    plt.figure(figsize=(10, 6))
    for i, (algo, group) in enumerate(df.groupby("algorithm")):
        plt.plot(
            group["threshold"],
            group["estimated_algo_mem"],
            marker=markers[i % len(markers)],
            color=colors[i % len(colors)],
            label=algo,
            linewidth=2,
            markersize=8
        )
    plt.xlabel("Threshold (%)", fontsize=12)
    plt.ylabel("Memory (KB)", fontsize=12)
    plt.title(f"Memory Usage vs Threshold - {dataset_name.upper()}", fontsize=14, fontweight="bold")
    plt.legend(fontsize=10)
    plt.grid(True, linestyle="--", alpha=0.6)
    plt.tight_layout()
    plt.savefig(out_dir / f"{dataset_name}_memory.png", dpi=150, bbox_inches="tight")
    plt.close()
    
    # 3. Check Count vs Threshold
    plt.figure(figsize=(10, 6))
    for i, (algo, group) in enumerate(df.groupby("algorithm")):
        plt.plot(
            group["threshold"],
            group["check_count"],
            marker=markers[i % len(markers)],
            color=colors[i % len(colors)],
            label=algo,
            linewidth=2,
            markersize=8
        )
    plt.xlabel("Threshold (%)", fontsize=12)
    plt.ylabel("Check Count", fontsize=12)
    plt.title(f"Check Count vs Threshold - {dataset_name.upper()}", fontsize=14, fontweight="bold")
    plt.legend(fontsize=10)
    plt.grid(True, linestyle="--", alpha=0.6)
    plt.tight_layout()
    plt.savefig(out_dir / f"{dataset_name}_check_count.png", dpi=150, bbox_inches="tight")
    plt.close()
    
    # 4. FWUPs Size vs Threshold
    plt.figure(figsize=(10, 6))
    for i, (algo, group) in enumerate(df.groupby("algorithm")):
        plt.plot(
            group["threshold"],
            group["fwups_size"],
            marker=markers[i % len(markers)],
            color=colors[i % len(colors)],
            label=algo,
            linewidth=2,
            markersize=8
        )
    plt.xlabel("Threshold (%)", fontsize=12)
    plt.ylabel("FWUPs Size", fontsize=12)
    plt.title(f"FWUPs Size vs Threshold - {dataset_name.upper()}", fontsize=14, fontweight="bold")
    plt.legend(fontsize=10)
    plt.grid(True, linestyle="--", alpha=0.6)
    plt.tight_layout()
    plt.savefig(out_dir / f"{dataset_name}_fwups_size.png", dpi=150, bbox_inches="tight")
    plt.close()
    
    # 5. Combined metrics (2x2 subplots)
    fig, axes = plt.subplots(2, 2, figsize=(16, 12))
    fig.suptitle(f"All Metrics - {dataset_name.upper()}", fontsize=16, fontweight="bold")
    
    # Runtime
    for i, (algo, group) in enumerate(df.groupby("algorithm")):
        axes[0, 0].plot(group["threshold"], group["runtime"], 
                       marker=markers[i % len(markers)], color=colors[i % len(colors)],
                       label=algo, linewidth=2, markersize=6)
    axes[0, 0].set_xlabel("Threshold (%)")
    axes[0, 0].set_ylabel("Runtime (ms)")
    axes[0, 0].set_title("Runtime")
    axes[0, 0].legend()
    axes[0, 0].grid(True, linestyle="--", alpha=0.6)
    
    # Memory
    for i, (algo, group) in enumerate(df.groupby("algorithm")):
        axes[0, 1].plot(group["threshold"], group["estimated_algo_mem"],
                       marker=markers[i % len(markers)], color=colors[i % len(colors)],
                       label=algo, linewidth=2, markersize=6)
    axes[0, 1].set_xlabel("Threshold (%)")
    axes[0, 1].set_ylabel("Memory (KB)")
    axes[0, 1].set_title("Memory Usage")
    axes[0, 1].legend()
    axes[0, 1].grid(True, linestyle="--", alpha=0.6)
    
    # Check Count
    for i, (algo, group) in enumerate(df.groupby("algorithm")):
        axes[1, 0].plot(group["threshold"], group["check_count"],
                       marker=markers[i % len(markers)], color=colors[i % len(colors)],
                       label=algo, linewidth=2, markersize=6)
    axes[1, 0].set_xlabel("Threshold (%)")
    axes[1, 0].set_ylabel("Check Count")
    axes[1, 0].set_title("Check Count")
    axes[1, 0].legend()
    axes[1, 0].grid(True, linestyle="--", alpha=0.6)
    
    # FWUPs Size
    for i, (algo, group) in enumerate(df.groupby("algorithm")):
        axes[1, 1].plot(group["threshold"], group["fwups_size"],
                       marker=markers[i % len(markers)], color=colors[i % len(colors)],
                       label=algo, linewidth=2, markersize=6)
    axes[1, 1].set_xlabel("Threshold (%)")
    axes[1, 1].set_ylabel("FWUPs Size")
    axes[1, 1].set_title("FWUPs Size")
    axes[1, 1].legend()
    axes[1, 1].grid(True, linestyle="--", alpha=0.6)
    
    plt.tight_layout()
    plt.savefig(out_dir / f"{dataset_name}_combined.png", dpi=150, bbox_inches="tight")
    plt.close()
    
    logging.info(f"✓ Saved 5 plots for {dataset_name}")

logging.info(f"✓ All plots saved to {out_dir}/")
