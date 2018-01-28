def test_reverser():
    from reverser import reverse_it
    data = b'abcdef'
    reversed_data = reverse_it(data)
    assert len(data) == len(reversed_data)
    assert reversed_data == b'fedcba'

def test_reverser_fast():
    from reverser import reverse_it_fast
    data = b'abcdef'
    reversed_data = reverse_it_fast(data)
    assert len(data) == len(reversed_data)
    assert reversed_data == b'fedcba'
