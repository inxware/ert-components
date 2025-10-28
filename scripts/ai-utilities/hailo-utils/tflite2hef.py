import click

@click.command()
@click.option('--hw_arch', default="hailo8", type=click.Choice(("hailo8", "hailo8l", "hailo8r")), help="Hailo target. Default \"hailo8\"")
@click.option('-o', '--output', type=click.Path(writable=True), required=True, help="Output filename")
@click.argument("model_name", type=click.STRING)
@click.argument("model_path", type=click.Path(exists=True, readable=True, resolve_path=True))
def cli(hw_arch: str, output: str, model_name: str, model_path: str):
    if model_path.split('.')[-1] != "tflite":
        raise click.BadArgumentUsage("The input model file must end with \".tflite\"!")
    if output.split('.')[-1] != "hef":
        raise click.BadArgumentUsage("The output file must end with \".hef\"!")
    import tensorflow as tf
    from hailo_sdk_client import ClientRunner

    # Parse TFLite model to HAR
    runner = ClientRunner(hw_arch=hw_arch)
    hn, npz = runner.translate_tf_model(model_path, model_name)

    # Optimise
    pass

    # Compilation
    hef = runner.compile()
    with open(output, "wb") as f:
        f.write(hef)

if __name__ == '__main__':
    cli()
