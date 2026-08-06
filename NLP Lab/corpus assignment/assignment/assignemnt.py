import os
import re
import random
from collections import defaultdict, Counter

START = "<s>"
END = "</s>"
MAX_WORDS = 50

def load_corpus(file_name):
    with open(file_name, "r", encoding="utf-8", errors="ignore") as file:
        return file.read()


def clean_gutenberg(text):
    text = re.sub(r"\*\*\* START OF .*?\*\*\*", " ", text,
                  flags=re.IGNORECASE | re.DOTALL)
    text = re.sub(r"\*\*\* END OF .*", " ", text,
                  flags=re.IGNORECASE | re.DOTALL)
    return text


def split_sentences(text):
    text = clean_gutenberg(text)
    text = re.sub(r"<[^>]+>", " ", text)
    text = re.sub(r"\s+", " ", text).strip()
    return re.split(r"[.!?]+", text)


def tokenize(text):
    return re.findall(r"[a-z]+", text.lower())


def preprocess_corpus(text):
    sentences = []
    for sentence in split_sentences(text):
        words = tokenize(sentence)
        if words:
            sentences.append(words)
    return sentences


def build_vocabulary(sentences):
    vocabulary = set()
    for sentence in sentences:
        vocabulary.update(sentence)
    vocabulary.add(END)
    return sorted(vocabulary)


def build_unigram_counts(sentences):
    counts = Counter()
    for sentence in sentences:
        counts.update(sentence)
        counts[END] += 1
    return counts


def build_ngram_counts(sentences, n):
    model = defaultdict(Counter)
    for sentence in sentences:
        padded = [START] * (n - 1) + sentence + [END]
        for i in range(len(padded) - n + 1):
            history = tuple(padded[i:i + n - 1])
            next_word = padded[i + n - 1]
            model[history][next_word] += 1
    return model


def mle_ngram_probability(model, history, word):
    counts = model[history]
    return counts[word] / sum(counts.values())


def mle_unigram_probability(unigram_counts, word):
    return unigram_counts[word] / sum(unigram_counts.values())


def laplace_ngram_probability(model, history, word, vocab_size):
    counts = model.get(history, Counter())
    return (counts.get(word, 0) + 1) / (sum(counts.values()) + vocab_size)


def laplace_unigram_probability(unigram_counts, word, vocab_size):
    total_words = sum(unigram_counts.values())
    return (unigram_counts.get(word, 0) + 1) / (total_words + vocab_size)


def model_name(n):
    return {4: "fourgram", 3: "trigram", 2: "bigram", 1: "unigram"}[n]

#backoff
def candidates_without_laplace(models, unigram_counts, history_words, highest_n):
    for n in range(highest_n, 1, -1):
        history = tuple(history_words[-(n - 1):])
        if history in models[n]:
            candidates = {}
            for word in models[n][history]:
                candidates[word] = mle_ngram_probability(models[n], history, word)
            return candidates, n, history

    candidates = {}
    for word in unigram_counts:
        candidates[word] = mle_unigram_probability(unigram_counts, word)
    return candidates, 1, tuple()

#backoff with lapplace smoothing
def candidates_with_laplace(models, unigram_counts, history_words, vocabulary, highest_n):
    vocab_size = len(vocabulary)
    for n in range(highest_n, 1, -1):
        history = tuple(history_words[-(n - 1):])
        if history in models[n]:
            candidates = {}
            for word in models[n][history]:
                candidates[word] = laplace_ngram_probability(
                    models[n], history, word, vocab_size
                )
            return candidates, n, history

    candidates = {}
    for word in unigram_counts:
        candidates[word] = laplace_unigram_probability(unigram_counts, word, vocab_size)
    return candidates, 1, tuple()


def choose_next_word(candidates, mode):
    if mode == "max":
        return max(candidates, key=candidates.get)
    words = list(candidates.keys())
    weights = list(candidates.values())
    return random.choices(words, weights=weights, k=1)[0]


def clean_seed(seed_text, vocabulary):
    words = tokenize(seed_text)
    words = [word for word in words if word in vocabulary]
    return words[-3:]


def format_sentence(words):
    if not words:
        return ""
    return " ".join(words).capitalize() + "."


def generate_sentence(models, unigram_counts, vocabulary, seed_text,
                      highest_n, mode, use_laplace):
    seed_words = clean_seed(seed_text, vocabulary)
    if not seed_words:
        seed_words = [START] * (highest_n - 1)

    output_words = [word for word in seed_words if word != START]
    history_words = [START] * (highest_n - 1) + seed_words
    first_info = None

    for step in range(MAX_WORDS):
        if use_laplace:
            candidates, used_n, history = candidates_with_laplace(
                models, unigram_counts, history_words, vocabulary, highest_n
            )
        else:
            candidates, used_n, history = candidates_without_laplace(
                models, unigram_counts, history_words, highest_n
            )

        next_word = choose_next_word(candidates, mode)
        prob = candidates[next_word]
        if step == 0:
            first_info = (used_n, history, next_word, prob)
        if next_word == END:
            break
        output_words.append(next_word)
        history_words.append(next_word)

    return format_sentence(output_words), first_info


def print_info(info):
    used_n, history, word, prob = info
    history_text = " ".join(history) if history else "general corpus frequency"
    print("Used    :", model_name(used_n))
    print("History :", history_text)
    print("Next    :", word, "| score =", round(prob, 6))


def get_highest_ngram_choice():
    print("\nChoose highest N-gram model:")
    print("2 = Bigram only")
    print("3 = Trigram with Bigram backoff")
    print("4 = Fourgram with Trigram and Bigram backoff")
    choice = input("Enter 2, 3, or 4 [default 4]: ").strip()
    return int(choice) if choice in ["2", "3", "4"] else 4


def main():
    corpus_file = input("Enter corpus file name [alice_in_wonderland.txt]: ").strip()
    if corpus_file == "":
        corpus_file = "alice_in_wonderland.txt"

    if not os.path.exists(corpus_file):
        print("\nCorpus file not found:", corpus_file)
        print("Put the corpus .txt file in the same folder as this Python file.")
        return

    sentences = preprocess_corpus(load_corpus(corpus_file))
    if not sentences:
        print("No valid text found in corpus.")
        return

    vocabulary = build_vocabulary(sentences)
    unigram_counts = build_unigram_counts(sentences)
    models = {2: build_ngram_counts(sentences, 2),
              3: build_ngram_counts(sentences, 3),
              4: build_ngram_counts(sentences, 4)}
    highest_n = get_highest_ngram_choice()

    print("\nCorpus loaded successfully.")
    print("Total sentences:", len(sentences))
    print("Vocabulary size:", len(vocabulary))
    print("Type q to quit.")

    while True:
        seed = input("\nEnter 1, 2, or 3 starting words: ").strip()
        if seed.lower() == "q":
            print("Goodbye!")
            break
        if seed and not clean_seed(seed, vocabulary):
            print("Note: input word not found in corpus, so sentence starts normally.")

        no_random, _ = generate_sentence(models, unigram_counts, vocabulary,
                                         seed, highest_n, "random", False)
        no_max, no_info = generate_sentence(models, unigram_counts, vocabulary,
                                            seed, highest_n, "max", False)
        yes_random, _ = generate_sentence(models, unigram_counts, vocabulary,
                                          seed, highest_n, "random", True)
        yes_max, yes_info = generate_sentence(models, unigram_counts, vocabulary,
                                              seed, highest_n, "max", True)

        print("\nWITHOUT Laplace smoothing / MLE only:")
        print("Random:", no_random)
        print("Max   :", no_max)
        print_info(no_info)

        print("\nWITH add-one Laplace smoothing:")
        print("Random:", yes_random)
        print("Max   :", yes_max)
        print_info(yes_info)


if __name__ == "__main__":
    main()