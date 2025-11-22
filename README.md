<h1>BMP Steganography Tool</h1>

<p>A C-based tool to hide secret files in BMP images using LSB (Least Significant Bit) steganography, and retrieve them safely using a validation magic string.</p>

<h2>Features</h2>
<ul>
    <li>Encode secret file data into BMP images.</li>
    <li>Hide secret file extension and size securely.</li>
    <li>Decode hidden files reliably using a magic string.</li>
    <li>Supports BMP images as source and stego output.</li>
    <li>Command-line interface for encoding and decoding.</li>
</ul>

<h2>Usage</h2>
<h3>Encoding:</h3>
<pre><code>./a.out -e source.bmp secret.txt [stego.bmp]</code></pre>
<p>If <code>stego.bmp</code> is not provided, output defaults to <code>stego.bmp</code>.</p>

<h3>Decoding:</h3>
<pre><code>./a.out -d stego.bmp [output_filename]</code></pre>
<p>If <code>output_filename</code> is not provided, output defaults to <code>new</code> with the original extension.</p>

<h2>Project Structure</h2>
<ul>
    <li><code>encode.c / encode.h</code> — Encoding functions and BMP handling.</li>
    <li><code>decode.c / decode.h</code> — Decoding pipeline and LSB extraction.</li>
    <li><code>types.h</code> — Custom types and status enums.</li>
    <li><code>common.h</code> — Magic string definition.</li>
    <li><code>main.c</code> — CLI interface and operation selection.</li>
</ul>

</body>
</html>
