# Drawshield-Code

Heraldry is the ancient art of creating recognisable and distinctive colours and patterns to represent a person or an organisation, usually in the form of a design drawn on to the shape of an armoured Knight's shield. Hundreds of years ago the way that these shield designs are described was formalised into a language known as _blazonry_, in which the description of a particular shield design is known as a _blazon_.

The code in the DrawShield repository is a PHP program that will take as input a heraldic blazon and create the corresponding graphical image of the shield. It forms the core of the functionality provided by the https://drawshield.net website.

It is primarily intended to work as a server side service via a simple stateless API call, either as a POST or GET request (although there is a limited command line interface that can be used locally, for debugging or batch creation). The default image format returned is SVG, although the code includes hooks to call out to an external program to convert to other formats such as PNGor jpeg.

The program code is of highly variable quality, some of it dates back over 7 years and was my first major PHP project, using PHP 5. The program was subsequently ported to later versions of PHP and newer features tend to have a more Object Oriented approach. There is documentation of equally variable quality in the wiki of this repository.

## Example Use

Install the code on a web server supporting PHP 8.1, for example in a directory ```/include/drawshield``` and invoke it with a GET API call in the following format:

```http://<your-server->/include/drawshield/drawshield.php?blazon=<URL-encoded-text-blazon>```

For a real example try:

```https://drawshield.net/include/drawshield.php?blazon=Azure,%20a%20bend%20or```

This will return an SVG file of a blue shield with a yellow diagonal band from top left to bottom right, which in most modern browsers will be displayed as an image like this:

![Azure a bend or](/examples/azure-a-bend-or.png)

Blazonry however has much greater expressive power and literally billions of different images can be created. There are over 12,000 example images, each complete with the blazon on the [DrawShield Gallery Page](https://drawshield.net/gallery/index.html):

![DrawShield Gallery](/examples/gallery.png)

Please check out the Wiki for an overview of the code organisation and a fairly random scattering of other documentation.

And please get in touch if you can help develop DrawShield further or have bug reports and feature suggestions.

