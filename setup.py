from setuptools import Extension, setup

c_extension = Extension(
    name="reverser",
    sources=["example.cpp"],
    language="cpp"
)

setup(
    name='reverser',
    author='Janis Lesinskis',
    description='buffer interface example',
    ext_modules=[c_extension]
)
