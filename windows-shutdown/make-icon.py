from PIL import Image
from pathlib import Path

# English comments required: script finds all PNG files that start with
# "icon-" under the assets directory and writes same-named .ico files
# containing multiple standard icon sizes.

SIZES = [(16, 16), (24, 24), (32, 32), (48, 48), (64, 64), (128, 128), (256, 256)]


def make_icon(png_path: Path) -> None:
	"""Create an .ico file next to the given png_path containing multiple sizes."""
	img = Image.open(png_path)
	# Place generated .ico files into the project's `icons/` directory rather
	# than next to the source PNG in `assets/`.
	base = Path(__file__).parent
	icons_dir = base / 'icons'
	icons_dir.mkdir(parents=True, exist_ok=True)
	ico_path = icons_dir / (png_path.stem + '.ico')
	img.save(ico_path, sizes=SIZES)
	print(f"Saved {ico_path}")


def main() -> None:
	base = Path(__file__).parent
	assets = base / 'assets'
	if not assets.exists():
		print(f"Assets directory not found: {assets}")
		return

	pattern = 'icon-*.png'
	files = list(assets.glob(pattern))
	if not files:
		print(f"No files matching {pattern} in {assets}")
		return

	for p in files:
		try:
			make_icon(p)
		except Exception as e:
			print(f"Failed to create icon for {p}: {e}")


if __name__ == '__main__':
	main()