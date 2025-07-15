# web_template_demo

A new Fern project created with `fern sprout`.

## Getting Started

1. **Run the project:**
   ```bash
   fern fire
   ```

2. **Build for web:**
   ```bash
   fern fire -p web
   ```

3. **Build for Linux:**
   ```bash
   fern fire -p linux
   ```

## Project Structure

```
web_template_demo/
├── lib/           # Main source code
├── web/           # Web platform specific files
│   └── template.html  # Customizable HTML template for web builds
├── linux/         # Linux platform specific files
├── assets/        # Images, fonts, etc.
├── examples/      # Example code
└── fern.yaml      # Project configuration
```

## Web Template Customization

The `web/template.html` file is used as the HTML template for web builds. You can customize:

- Page title and meta tags
- CSS styling and layout
- Additional JavaScript libraries
- Canvas container structure

The template uses `{{{ SCRIPT }}}` as a placeholder for the generated JavaScript code.

## Learn More

- [Fern Documentation](https://github.com/your-repo/fern)
- [Examples](./examples/)
