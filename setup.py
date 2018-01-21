from setuptools import Extension, setup

c_extension = Extension(
    sources="example.cpp",
    language="cpp"
)

setup(
    name='reverser',
    author='Janis Lesinskis',
    description='buffer interface example'
)
