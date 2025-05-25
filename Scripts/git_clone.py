import sys
import re
from git import Repo, RemoteProgress
from tqdm import tqdm

class PhaseAwareProgress(RemoteProgress):
    def __init__(self):
        super().__init__()
        self.pbar = None
        self.current_phase = "Initializing"
        self.received_objects = 0
        self.total_objects = 0

    def update(self, op_code, cur_count, max_count=None, message=''):
        # Determine phase using op_code
        if op_code & self.COUNTING:
            self._update_phase("Counting Objects", message)
        elif op_code & self.COMPRESSING:
            self._update_phase("Compressing", message)
            self._parse_object_counts(message)
        elif op_code & self.RECEIVING:
            self._update_phase("Receiving Objects", message)
            self._parse_object_counts(message)
        elif op_code & self.RESOLVING:
            self._update_phase("Resolving Deltas", message)

        # Update progress
        self._update_progress(cur_count, max_count)

    def _update_phase(self, phase_name, message):
        if self.current_phase != phase_name:
            self.current_phase = phase_name
            print(f"\n\033[33m• {phase_name}\033[0m")  # Yellow phase header
            if self.pbar:
                self.pbar.close()
            self.pbar = tqdm(
                total=100,
                desc=f"{self.current_phase:16}",
                bar_format="{l_bar}{bar:40}| {n:3d}%",
                dynamic_ncols=True,
                colour='CYAN'
            )

    def _parse_object_counts(self, message):
        match = re.search(r'(\d+)\/(\d+)', message)
        if match:
            self.received_objects = int(match.group(1))
            self.total_objects = int(match.group(2))

    def _update_progress(self, cur_count, max_count):
        if max_count and max_count > 0:
            progress = int(cur_count / max_count * 100)
            if self.pbar:
                delta = progress - self.pbar.n
                if delta > 0:
                    self.pbar.update(delta)

    def close(self):
        if self.pbar:
            self.pbar.close()

def clone_repo(repo_url, local_path):
    progress = PhaseAwareProgress()
    try:
        print(f"\n\033[1mCloning {repo_url}\033[0m")
        Repo.clone_from(repo_url, local_path, progress=progress)
        print(f"\n\033[32m✓ Clone completed successfully!\033[0m")
        return 0
    except Exception as e:
        print(f"\n\033[31m✗ Clone failed: {e}\033[0m")
        return 1
    finally:
        progress.close()

if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("Usage: python git_clone.py <repository_url> <target_dir>")
        sys.exit(1)

    sys.exit(clone_repo(sys.argv[1], sys.argv[2]))
