# Drawshield-Code

Heraldry is the ancient art of creating recognisable and distinctive colours and patterns to represent a person or an organisation, usually in the form of a design drawn on to the shape of an armoured Knight's shield. Hundreds of years ago the way that these shield designs are described was formalised into a language known as _blazonry_, in which the description of a particular shield design is known as a _blazon_.

The code in the DrawShield repository is a PHP program that will take as input a heraldic blazon and create the corresponding graphical image of the shield. It forms the core of the functionality provided by the https://drawshield.net website.

It is primarily intended to work as a server side service via a simple stateless API call, either as a POST or GET request (although there is a limited command line interface that can be used locally, for debugging or batch creation). The default image format returned is SVG, although if ImageMagick and the corresponding PHP libraries are installed on the server then PNG and JPG are also supported.

The program code is of highly variable quality, some of it dates back over 7 years and was my first major PHP project, using PHP 5. The program was subsequently ported to PHP 7 and newer features tend to have a more Object Oriented approach. I intend to provide at least some overview documentation in the Wiki of this repository.

## Example Use

Install the code on a web server supporting PHP 7, for example in a directory ```/include/drawshield``` and invoke it with a GET API call in the following format:

```http://<your-server->/include/drawshield/drawshield.php?blazon=<URL-encoded-text-blazon>```

For a real example try:

```https://drawshield.net/include/drawshield.php?blazon=Azure,%20a%20bend%20or```

This will return an SVG file of a blue shield with a yellow diagonal band from top left to bottom right, which in most modern browsers will be displayed as an image like this:

![Azure a bend or](/examples/azure-a-bend-or)

Blazonry however has much greater expressive power and literally billions of different images can be created. There are over 1,500 example images, each complete with the blazon on the [DrawShield Gallery Page](https://drawshield.net/gallery/index.html):

![DrawShield Gallery](/examples/gallery.png)


